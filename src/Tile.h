#ifndef _TILE_H
#define _TILE_H

#include "SDL.h"
#include "SDL_image.h"
#include "LTexture.h"
#include <stdio.h>

//Tile Generation
// Code obtained from lazyfoo at lazyfoo.net

//Screen Dimension Constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

//Tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;
//Tile sprites
//Testing Sprites. Plan to change later to environmental tiles
const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;



SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];

class Tile
{
	public:
		Tile(int x, int y, int tileType);

		void render( SDL_Rect& camera);

		int getType();
		SDL_Rect getBox();

private:
	//the attributes of the tile
	SDL_Rect mBox;
	//The tile type
	int mType;
};
//inits SDL and creates window
bool init();
//Loads media
bool loadMedia();
//Shuts down SDL and frees media
void close();
//Box collision for camera
bool checkCollision( SDL_Rect a, SDL_Rect b);
//collision against set of tils
//bool touchesWall( SDL_Rect box, Tile* tiles[]);

//Sets tiles from tile map
bool setTiles ( Tile *tiles[]);


#endif