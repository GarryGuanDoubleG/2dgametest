#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <string>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "graphics.h"
/**
* @brief sprite structure used for clipping images for sprites and 
*  loading / drawing image into the game 
**/
typedef struct
{
	int refCount;/**<keeps track of how many times sprite was referenced**/
	char *filename;/**<name of image sprite has loaded*/
	SDL_Texture *image;/**<Texture created when image is loaded*/
	float imageW, imageH;
	float frameW, frameH;
	int fpl;
}Sprite;

typedef Sprite Text;

//Draws Mouse
void DrawMouse2();
void InitMouse2();

extern Sprite* Sprite_Mouse;
void sprite_initialize_system(int max_sprites);
void sprite_close_system();

Vec2d get_mouse_pos();
Sprite *Sprite_Load(char *filename, int img_width, int img_height, int frameW, int frameH);
Sprite *Sprite_Load_Text(TTF_Font *font, char *text, SDL_Color color);

void Sprite_Free(Sprite **sprite);
void Sprite_Draw(Sprite *sprite, int frame, SDL_Renderer *renderer, int drawX, int drawY);
void Sprite_Text_Draw(Sprite *text, Vec2d drawPos, int alpha);
#endif