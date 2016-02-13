#include "Tile.h"

const int TOTAL_TILES = 210;//15x14 Tile Map
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TILE_GRASS1 = 0;//type. Should be made into an enum or macro

Tile *tile_list = NULL;
SDL_Texture * tile_sprite_grass = NULL;

void tile_init_system(){
	int i;
	if(TOTAL_TILES == 0){
		printf("Why is total tiles 0?");
		return;
	}

	tile_list = (Tile*) malloc(sizeof(Tile)*TOTAL_TILES);
	memset(tile_list,0, sizeof(Tile) * TOTAL_TILES);

	SDL_Rect temp = {0,0,0,0};
	for(i = 0; i < TOTAL_TILES; i++){
		tile_list[i].mBox = temp;
	}

	tile_load("images/Grass01.png");
	tile_set();
	tile_render(NULL);

	atexit(tile_close_system);
}

void tile_load(char *filename){

	SDL_Surface * load_surface = IMG_Load(filename);
	if(!load_surface)
	{
		printf("Could not load grass tile sprite");
		return;
	}
	SDL_SetColorKey( load_surface, SDL_TRUE, SDL_MapRGBA( load_surface->format, 0, 0xFF, 0xFF,0xFF ) );

	tile_sprite_grass = SDL_CreateTextureFromSurface(__gt_graphics_renderer, load_surface);
	if(!tile_sprite_grass){
		printf("Unable to create New Texture from Sprite Surface");
		return ;
	}
}

void tile_set(){
	int i;
	int x = 0,y = 0;
	Tile * tile;
	for( i = 0; i < TOTAL_TILES; i++){
		tile = (tile_list+i);
		tile->mBox.x = x;
		tile->mBox.y = y;
		tile->mBox.w = TILE_WIDTH;
		tile->mBox.h = TILE_HEIGHT;
		tile->mType = TILE_GRASS1;//filler

		x += TILE_WIDTH;		
		if (x >= SCREEN_WIDTH){
			x = 0;
			y += TILE_HEIGHT;
		}
	}
}

void tile_free(Tile *tile){
	tile->mType = 0;
}

void tile_render(SDL_Rect *camera){
	int i;
	for( i = 0; i < TOTAL_TILES; i++){
		Tile * tile = (tile_list + i);
		if(tile == NULL){
			printf("Tile is null while rendering");
			return ;
		}
		SDL_Rect src = { tile->mBox.x,tile->mBox.y, 
						 TILE_HEIGHT, 
						 TILE_WIDTH};
		SDL_Rect dest = { tile->mBox.x,tile->mBox.y, TILE_HEIGHT, TILE_WIDTH};

		SDL_RenderCopy(__gt_graphics_renderer,tile_sprite_grass, NULL, &dest);
	}
}

void tile_close_system(){
	int i;
	for( i = 0; i < TOTAL_TILES; i++)
		tile_free(tile_list+i);

	SDL_DestroyTexture(tile_sprite_grass);
}
