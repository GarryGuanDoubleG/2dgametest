#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "graphics.h"

#define MaxSprites    255

const int SCREEN_HEIGHT = 720;
const int SCREEN_WIDTH = 1280;

SDL_Surface *buffer; /*pointer to the background image buffer*/
SDL_Surface *videobuffer; /*pointer to the draw buffer*/
SDL_Rect Camera; /*x & y are the coordinates for the background map, w and h are of the screen*/

static Uint32 g_graphics_now;					/*the current time since program started*/
static Uint32 g_graphics_then;
static Uint32 g_graphics_diff;
static Uint32 g_graphics_frame_delay = 30;

SDL_Window   *   g_graphics_main_window = NULL;
SDL_Renderer *   g_graphics_renderer = NULL;

void graphics_close();

void Init_Graphics(
	char *windowName,
    int viewWidth,
    int viewHeight,
    int renderWidth,
    int renderHeight,
    int fullscreen)
{
    Uint32 flags = 0;

	SDL_Rect camera = {0,0, SCREEN_WIDTH, SCREEN_HEIGHT};
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Unable to initilaize SDL system: %s",SDL_GetError());
        return;
    }
    atexit(SDL_Quit);
    if (fullscreen)
    {
        if (renderWidth == 0)
        {
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
        else
        {
            flags |= SDL_WINDOW_FULLSCREEN;
        }
    }
    g_graphics_main_window = SDL_CreateWindow(windowName,
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             renderWidth, renderHeight,
                             flags);

    if (!g_graphics_main_window)
    {
        printf("failed to create main window: %s",SDL_GetError());
        graphics_close();
        return;
    }
    
    g_graphics_renderer = SDL_CreateRenderer(g_graphics_main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!g_graphics_renderer)
    {
        printf("failed to create renderer: %s",SDL_GetError());
        graphics_close();
        return;
    }
    
    SDL_SetRenderDrawColor(g_graphics_renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_graphics_renderer);
    SDL_RenderPresent(g_graphics_renderer);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(g_graphics_renderer, renderWidth, renderHeight);
}

void graphics_frame_delay()
{
	g_graphics_then = g_graphics_now;
	g_graphics_now = SDL_GetTicks();
	
	g_graphics_diff = g_graphics_now - g_graphics_then;

	if(g_graphics_diff < g_graphics_frame_delay)
	{
		SDL_Delay(g_graphics_frame_delay - g_graphics_diff);
	}
}

void graphics_next_frame()
{
  SDL_RenderPresent(g_graphics_renderer);
  graphics_frame_delay();
}

void graphics_close()
{
    if (g_graphics_renderer)
    {
        SDL_DestroyRenderer(g_graphics_renderer);
    }
    if (g_graphics_main_window)
    {
        SDL_DestroyWindow(g_graphics_main_window);
    }

    g_graphics_main_window = NULL;
    g_graphics_renderer = NULL;
}

SDL_Renderer * Graphics_Get_Renderer(){
	return g_graphics_renderer;
}
