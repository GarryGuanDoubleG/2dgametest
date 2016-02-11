#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <string>
#include <string.h>
#include <stdlib.h>

#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"

typedef struct
{
	int refCount;
	char filename[128];
	SDL_Texture *image;
	int imageW, imageH;
	int frameW, frameH;
	int fpl;
}Sprite2;
//Draws Mouse
void DrawMouse2();
void InitMouse2();

extern Sprite2* Sprite_Mouse;
void sprite_initialize_system();
void sprite_close_system();

Sprite2 *sprite_load(char *filename, int width, int height);
void sprite_free(Sprite2 *sprite);
void sprite_draw(Sprite2 *sprite, int frame, SDL_Renderer *renderer, int drawX, int drawY);

#endif