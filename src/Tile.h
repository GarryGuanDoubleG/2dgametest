#ifndef _TILES_H_
#define _TILES_H_
#include "graphics.h"

typedef struct{
public:
	void (*render)(SDL_Rect & camera);

	int getType();

	SDL_Rect getBox();

	SDL_Rect mBox;

	//Type of tile
	int mType;

}Tile;

void Tile_Init(Tile* tile);


#endif