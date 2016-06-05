#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <string>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "graphics.h"
#include "camera.h"
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
void Draw_Mouse();
void Init_Mouse();

extern Sprite* Sprite_Mouse;
void Sprite_Initialize_System(int max_sprites);
void Sprite_Close_System();

Vec2d Get_Mouse_Pos();
Sprite *Sprite_Load(char *filename, int img_width, int img_height, int frameW, int frameH);
Sprite *Sprite_Load_Text(TTF_Font *font, char *text, SDL_Color color);

void Sprite_Free(Sprite **sprite);

void Sprite_Draw(Sprite *sprite, int frame, Vec2d draw_pos);

void Sprite_Text_Draw(Sprite *text, Vec2d drawPos, int alpha);

#endif