#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <string>
#include <string.h>
#include <stdlib.h>

#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
/**
* @brief sprite structure used for clipping images for sprites and 
*  loading / drawing image into the game 
**/
typedef struct
{
	int refCount;/**<keeps track of how many times sprite was referenced**/
	char filename[128];/**<name of image sprite has loaded*/
	SDL_Texture *image;/**<Texture created when image is loaded*/
	float imageW, imageH;
	float frameW, frameH;
	int fpl;
}Sprite2;

//Draws Mouse
void DrawMouse2();
void InitMouse2();

extern Sprite2* Sprite_Mouse;
void sprite_initialize_system(int max_sprites);
void sprite_close_system();

Vec2d get_mouse_pos();
Sprite2 *sprite_load(char *filename, int img_width, int img_height, int frameW, int frameH);
void sprite_free(Sprite2 **sprite);
void sprite_draw(Sprite2 *sprite, int frame_horizontal, int frame_vertical, SDL_Renderer *renderer, int drawX, int drawY);

#endif