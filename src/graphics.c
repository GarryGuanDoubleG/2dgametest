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

Uint32 NOW;					/*the current time since program started*/

SDL_Window   *   __gt_graphics_main_window = NULL;
SDL_Renderer *   __gt_graphics_renderer = NULL;
SDL_Texture  *   __gt_graphics_texture = NULL;
SDL_Surface  *   __gt_graphics_surface = NULL;
SDL_Surface  *   __gt_graphics_temp_buffer = NULL;
//gg edit
static SDL_Rect		graphics_player_camera;


/*some data on the video settings that can be useful for a lot of functions*/
static int __gt_bitdepth;
static Uint32 __gt_rmask;
static Uint32 __gt_gmask;
static Uint32 __gt_bmask;
static Uint32 __gt_amask;

void gt_graphics_close();

void Init_Graphics(
	char *windowName,
    int viewWidth,
    int viewHeight,
    int renderWidth,
    int renderHeight,
    float bgcolor[4],
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
    __gt_graphics_main_window = SDL_CreateWindow(windowName,
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             renderWidth, renderHeight,
                             flags);

    if (!__gt_graphics_main_window)
    {
        printf("failed to create main window: %s",SDL_GetError());
        gt_graphics_close();
        return;
    }
    
    __gt_graphics_renderer = SDL_CreateRenderer(__gt_graphics_main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!__gt_graphics_renderer)
    {
        printf("failed to create renderer: %s",SDL_GetError());
        gt_graphics_close();
        return;
    }
    
    SDL_SetRenderDrawColor(__gt_graphics_renderer, 0, 0, 0, 255);
    SDL_RenderClear(__gt_graphics_renderer);
    SDL_RenderPresent(__gt_graphics_renderer);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(__gt_graphics_renderer, renderWidth, renderHeight);

    __gt_graphics_texture = SDL_CreateTexture(
        __gt_graphics_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        renderWidth, renderHeight);
    if (!__gt_graphics_texture)
    {
        printf("failed to create screen texture: %s",SDL_GetError());
        gt_graphics_close();
        return;
    };
    
    SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ARGB8888,
                                    &__gt_bitdepth,
                                    &__gt_rmask,
                                    &__gt_gmask,
                                    &__gt_bmask,
                                    &__gt_amask);

    
    __gt_graphics_surface = SDL_CreateRGBSurface(0, renderWidth, renderHeight, __gt_bitdepth,
                                        __gt_rmask,
                                    __gt_gmask,
                                    __gt_bmask,
                                    __gt_amask);
    buffer = SDL_CreateRGBSurface(0, renderWidth, renderHeight, __gt_bitdepth,
                                                 __gt_rmask,
                                                 __gt_gmask,
                                                 __gt_bmask,
                                                 __gt_amask);    
    if (!__gt_graphics_surface)
    {
        printf("failed to create screen surface: %s",SDL_GetError());
        gt_graphics_close();
        return;
    }
	//gg edit
	graphics_player_camera = camera;
        
    atexit(gt_graphics_close);
    printf("graphics initialized\n");
}

void gt_graphics_render_surface_to_screen(SDL_Surface *surface,SDL_Rect srcRect,int x,int y)
{
    SDL_Rect dstRect;
    SDL_Point point = {1,1};
    int w,h;
    if (!__gt_graphics_texture)
    {
        printf("gt_graphics_render_surface_to_screen: no texture available");
        return;
    }
    if (!surface)
    {
        printf("gt_graphics_render_surface_to_screen: no surface provided");
        return;
    }
    SDL_QueryTexture(__gt_graphics_texture,
                     NULL,
                     NULL,
                     &w,
                     &h);
    /*check if resize is needed*/
    if ((surface->w > w)||(surface->h > h))
    {
        SDL_DestroyTexture(__gt_graphics_texture);
        __gt_graphics_texture = SDL_CreateTexture(__gt_graphics_renderer,
                                                   __gt_graphics_surface->format->format,
                                                   SDL_TEXTUREACCESS_STREAMING, 
                                                   surface->w,
                                                   surface->h);
        if (!__gt_graphics_texture)
        {
            printf("gt_graphics_render_surface_to_screen: failed to allocate more space for the screen texture!");
            return;
        }
    }
    SDL_SetTextureBlendMode(__gt_graphics_texture,SDL_BLENDMODE_BLEND);        
    SDL_UpdateTexture(__gt_graphics_texture,
                      &srcRect,
                      surface->pixels,
                      surface->pitch);
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = srcRect.w;
    dstRect.h = srcRect.h;
	if(rect_collide(graphics_player_camera, dstRect))
	{
		SDL_RenderCopy(__gt_graphics_renderer,
                     __gt_graphics_texture,
                     &srcRect,
                     &dstRect);
	}
}


void ResetBuffer()
{
    
}

void NextFrame()
{
  Uint32 Then;
  SDL_RenderPresent(__gt_graphics_renderer);
  Then = NOW;									/*these next few lines  are used to show how long each frame takes to update.  */
  NOW = SDL_GetTicks();
/*  fprintf(stdout,"Ticks passed this frame: %i\n", NOW - Then);*/
  FrameDelay(33); /*this will make your frame rate about 30 frames per second.  If you want 60 fps then set it to about 15 or 16*/
}

void gt_graphics_close()
{
    if (__gt_graphics_texture)
    {
        SDL_DestroyTexture(__gt_graphics_texture);
    }
    if (__gt_graphics_renderer)
    {
        SDL_DestroyRenderer(__gt_graphics_renderer);
    }
    if (__gt_graphics_main_window)
    {
        SDL_DestroyWindow(__gt_graphics_main_window);
    }
    if (__gt_graphics_surface)
    {
        SDL_FreeSurface(__gt_graphics_surface);
    }
    if (__gt_graphics_temp_buffer)
    {
        SDL_FreeSurface(__gt_graphics_temp_buffer);
    }
    __gt_graphics_surface = NULL;
    __gt_graphics_main_window = NULL;
    __gt_graphics_renderer = NULL;
    __gt_graphics_texture = NULL;
    __gt_graphics_temp_buffer = NULL;
}


Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    /* Here p is the address to the pixel we want to retrieve*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;

    switch(surface->format->BytesPerPixel)
    {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /*shouldn't happen, but avoids warnings*/
    }
}



/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;

    switch(surface->format->BytesPerPixel)
    {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}


/*
  makes sure a minimum number of ticks is waited between frames
  this is to ensure that on faster machines the game won't move so fast that
  it will look terrible.
  This is a very handy function in game programming.
*/

void FrameDelay(Uint32 delay)
{
    static Uint32 pass = 100;
    Uint32 dif;
    dif = SDL_GetTicks() - pass;
    if(dif < delay)SDL_Delay( delay - dif);
    pass = SDL_GetTicks();
}

/*sets an sdl surface to all color.*/

void BlankScreen(SDL_Surface *buf,Uint32 color)
{
    SDL_LockSurface(buf);
    memset(buf->pixels, (Uint8)color,buf->format->BytesPerPixel * buf->w *buf->h);
    SDL_UnlockSurface(buf);
}
/*
 * This is the beginning of my Palette swapping scheme.  It checks the value
 * of the color it is given to see if the given color is PURE red, PURE green,
 * or PURE blue.  If it is, it takes the value as a percentage to apply to
 * the new color.  It returns either the old color untouched (if it wasn't a 
 * special case) or the new color.
 */

//GG edit

SDL_Window * get_gt_window(){
	return __gt_graphics_main_window;
}
SDL_Renderer * get_gt_renderer(){
	return __gt_graphics_renderer;
}
SDL_Texture * get_gt_texture(){
	return __gt_graphics_texture;
}
SDL_Surface * get_gt_Surface(){
	return __gt_graphics_surface;
}
SDL_Surface * get_gt_buffer(){
	return __gt_graphics_temp_buffer;
}

SDL_Renderer * Graphics_Get_Renderer(){
	return __gt_graphics_renderer;
}

SDL_Rect Graphics_Get_Player_Cam()
{
	return graphics_player_camera;
}

void graphics_update_player_cam(SDL_Rect player_cam)
{
	graphics_player_camera = player_cam;
	
}