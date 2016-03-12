#include "Tile.h"
#include "simple_logger.h"
#include <time.h>
#include <math.h>

const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES_X = SCREEN_WIDTH/TILE_WIDTH * 3;
const int TOTAL_TILES_Y = SCREEN_HEIGHT/TILE_HEIGHT * 3;
const int TOTAL_TILES = TOTAL_TILES_X * TOTAL_TILES_Y ;//make a square 3 by 3 region of size screen

Tile *tile_list = NULL;
Destructable_Tile *dest_tile_list = NULL;
Sprite2 * tile_sprite_grass = NULL;
Sprite2 * tile_sprite_tree = NULL;


void tile_init_system(){
	int i;
	if(TOTAL_TILES == 0){
		printf("Why is total tiles 0?");
		return;
	}
	tile_list = (Tile*) malloc(sizeof(Tile)*TOTAL_TILES);
	memset(tile_list,0, sizeof(Tile) * TOTAL_TILES);

	dest_tile_list = (Destructable_Tile*) malloc(sizeof(Destructable_Tile)*TOTAL_TILES);
	memset(tile_list,0, sizeof(Destructable_Tile) * TOTAL_TILES);

	tile_sprite_grass = tile_load("images/Grass01.png");
	tile_sprite_tree = tile_load(PATH_TILE_TREE);
	tile_set();
	tile_render();

	atexit(tile_close_system);
}

Sprite2 * tile_load(char *filename)
{
	Sprite2 * sprite = sprite_load(filename, TILE_WIDTH, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
	return sprite;
	/*SDL_Surface * load_surface = IMG_Load(filename);
	if(!load_surface)
	{
		printf("Could not load grass tile sprite");
		return;
	}
	SDL_SetColorKey( load_surface, SDL_TRUE, SDL_MapRGBA( load_surface->format, 0, 0xFF, 0xFF,0xFF ) );

	tile_sprite_grass = SDL_CreateTextureFromSurface(__gt_graphics_renderer, load_surface);
	if(!tile_sprite_grass){
		slog("Unable to create New Texture from Sprite Surface");
		return ;
	}*/

}

//applies to whole map
void tile_forest_gen(int start){
	int i;
	int x = 0, y = 0;
}

void tile_forest_gen()
{
	//start of top edge of forest
	int start = rand() % TOTAL_TILES_X -1; //rand num from 0 to end of first row
	tile_forest_gen(start);
	//start on left edge of forest
	start = rand() % TOTAL_TILES_Y + 1;
	start = start >= TOTAL_TILES_Y ? TOTAL_TILES_Y - 1 : start;
	start = start * TOTAL_TILES_X;
	tile_forest_gen(start);
	//right edge
	start = rand() % TOTAL_TILES_Y +1;
	start = start >= TOTAL_TILES_Y ? TOTAL_TILES_Y - 1 : start;
	start = (start *TOTAL_TILES_X) - 1;
	tile_forest_gen(start);
	//bottom edge
	start = rand() % TOTAL_TILES_X -1;
	start += TOTAL_TILES - TOTAL_TILES_X;
	tile_forest_gen(start);
}

void tile_set(){
	int i;
	int x = 0,y = 0;
	Tile * tile;
	//place the grass tile
	for( i = 0; i < TOTAL_TILES; i++){

		tile = (tile_list+i);
		tile->mBox.x = x;
		tile->mBox.y = y;
		tile->mBox.w = TILE_WIDTH;
		tile->mBox.h = TILE_HEIGHT;
		tile->mType = TILE_GRASS1;//filler
		
		dest_tile_list[i].mBox = tile->mBox;
		dest_tile_list[i].mType = TILE_TREE;
		x += TILE_WIDTH;		
		if(x >= TOTAL_TILES_X * TILE_WIDTH)
		{
			x = 0;
			slog("Total Tiles X is < X");
			y += TILE_HEIGHT;
		} 
	}

}

void tile_free(Tile *tile){
	tile->mType = 0;
}

void tile_render(){
	int i;
	SDL_Rect camera = graphics_get_player_cam();
	for( i = 0; i < TOTAL_TILES; i++){
		Tile * tile = (tile_list + i);
		if(tile == NULL){
			printf("Tile is null while rendering");
			return ;
		}
		if(rect_collide(graphics_get_player_cam(), tile->mBox))
		{
			SDL_Rect dest = { tile->mBox.x - camera.x,tile->mBox.y-camera.y, TILE_HEIGHT, TILE_WIDTH};
			SDL_RenderCopy(__gt_graphics_renderer,tile_sprite_grass->image, NULL, &dest);
			if(dest_tile_list[i].mType == TILE_TREE)
			{
				SDL_RenderCopy(__gt_graphics_renderer,tile_sprite_tree->image, NULL, &dest);
			}
		}
	}
}

void tile_close_system(){
	int i;
	for( i = 0; i < TOTAL_TILES; i++)
		tile_free(tile_list+i);

	SDL_DestroyTexture(tile_sprite_grass->image);
}
