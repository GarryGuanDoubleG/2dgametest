#include "Tile.h"

//code based on lazyfoo's tile generation

Tile::Tile(int x, int y, int tileType){
	
	mBox.x = x;
	mBox.y = y;
	mBox.h = TILE_HEIGHT;
	mBox.w = TILE_WIDTH;

	mType = tileType;

}

void Tile::render(SDL_Rect &camera){

	if(checkCollision(camera,mBox)){
		gTileTexture.render( mBox.x - camera.x, mBox.y - camera.y, &gTileClips[ mType ] );
	}

}

bool init(){

	return false;
}

bool loadMedia(){
	return false;
}