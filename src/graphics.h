#ifndef _GRAPHICS_
#define _GRAPHICS_
/*
 *			graphics.h
 *			Originaly written by Donald Kehoe for the NJIT Introduction to game development class
 *			This file contains data structures, constants and function prototypes needed to initialize SDL and
 *			to handle sprites and graphics.
 */

#include <stdio.h>
#include "vector.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "simple_logger.h"

/*color constants:*/
enum colors {Red = 1,Green = 2,Blue = 3,Yellow = 4,Orange = 5,Violet = 6,Brown = 7,Grey = 8,DarkRed = 9,DarkGreen = 10,
               DarkBlue = 11,DarkYellow = 12,DarkOrange = 13,DarkViolet = 14,DarkBrown = 15,DarkGrey = 16,LightRed =17,
               LightGreen = 18,LightBlue = 19,LightYellow = 20,LightOrange = 21,LightViolet = 22,LightBrown = 23,LightGrey = 24,
               Black = 25,White = 26,Tan = 27,Gold = 28,Silver = 29,YellowGreen = 30,Cyan = 31,Magenta = 32};

extern const int SCREEN_HEIGHT;
extern const int SCREEN_WIDTH;

extern SDL_Surface *buffer; /*pointer to the background image buffer*/
extern SDL_Surface *videobuffer; /*pointer to the draw buffer*/
extern SDL_Rect Camera; /*x & y are the coordinates for the background map, w and h are of the screen*/
extern int NumSprites;
extern Uint32 NOW;		

extern SDL_Window   *   __gt_graphics_main_window;
extern SDL_Renderer *   __gt_graphics_renderer;
extern SDL_Texture  *   __gt_graphics_texture;
extern SDL_Surface  *   __gt_graphics_surface;
extern SDL_Surface  *   __gt_graphics_temp_buffer;

void Init_Graphics(
	char *windowName,
    int viewWidth,
    int viewHeight,
    int renderWidth,
    int renderHeight,
    float bgcolor[4],
    int fullscreen);
void DrawPixel(SDL_Surface *screen, Uint8 R, Uint8 G, Uint8 B, int x, int y);
void BlankScreen(SDL_Surface *buf,Uint32 color);
void gt_graphics_render_surface_to_screen(SDL_Surface *surface,SDL_Rect srcRect,int x,int y);

Uint32 IndexColor(int color);
void FrameDelay(Uint32 delay);
void ResetBuffer();
void NextFrame();
void InitMouse();
void DrawMouse();
//player camera updated in player update
SDL_Rect graphics_get_player_cam();
void graphics_update_player_cam( SDL_Rect player_cam );
//gg edit
SDL_Renderer * graphics_get_renderer();


#endif
