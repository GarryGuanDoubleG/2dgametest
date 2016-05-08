#include "level.h"
#include "load.h"
#include "save.h"
#include "simple_logger.h"
#include "json_parse.h"
#include "graphics.h"
#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>

void Level_Editor_Move(SDL_Event *e);

void Level_Editor_Init()
{
	Vec2d em_cam_size;
	Vec2d em_cam_offset;

	int cam_width = (SCREEN_WIDTH * 3) / 4;
	int cam_height = (SCREEN_HEIGHT * 3) / 4;

	//offset camera to left
	int cam_pos_x = ((SCREEN_WIDTH - cam_width) /2 ) * -1;
	int cam_pos_y = ((SCREEN_HEIGHT - cam_height) /2 ) * -1;

	Vec2dSet(em_cam_size, cam_width, cam_height);
	Vec2dSet(em_cam_offset, cam_pos_x, cam_pos_y);

	Camera_SetSize(em_cam_size);
	Camera_SetPosition(em_cam_offset);

	tile_init_system(MODE_EDITOR);
}

void Level_Editor_Draw()
{
	//clear screen
	ResetBuffer();
    SDL_RenderClear(Graphics_Get_Renderer());//clear screen

	tile_draw();
	Draw_Mouse();

	NextFrame();
}

void Level_Editor_Mode()
{
	int done;
	const Uint8 *keys;
	SDL_Event e;

	keys = SDL_GetKeyboardState(NULL);
	done = Bool_False;

	Level_Editor_Init();

	do
	{

		Level_Editor_Draw();

		while( SDL_PollEvent( &e) != 0)
		{
			if(e.type == SDL_QUIT)
			{
				done = 1;
			}
			else
			{
				Level_Editor_Move(&e);
			}
		}

		if(keys[SDL_SCANCODE_ESCAPE])
		{
			Level_Save();
			done = 1;
		}
	}while(!done);
}

void Level_Load_New()
{
  tile_init_system(Bool_True); // allocate memory for tiles and generate map
  Player_Init(); //creates player entity
}

void Level_Load(int load)
{
	Dict * last_save;
	Dict * value;

	int player_tile;

	int *tile_map;
	int tile_row;
	int tile_col;

	if(load == Bool_False)
	{
		Level_Load_New();
		return;
	}

	last_save = load_dict_from_file("save.def");
	
	//get tile rows
	value = dict_get_hash_value(last_save, "TILE_ROWS");
	if(!value) return;
	tile_row = atoi((char *)value->keyValue);

	//get tile columns
	value = dict_get_hash_value(last_save, "TILE_COLUMNS");
	if(!value) return;
	tile_col = atoi((char *)value->keyValue);

	//get map
	value = dict_get_hash_value(last_save, "TILE_MAP");
	if(!value) return;
	tile_map = (int *)value->keyValue;

	//load map into game
	tile_load_from_def(tile_map); // allocate memory for tiles and generate map

	//player data should be stored in a hash
	//but only starting tile is being saved;
	value = dict_get_hash_value(last_save, "PLAYER_TILE");
	if(!value) return;
	Player_Load_from_Def(value); //creates player entity
}

void Level_Save()
{
	int tile_count;
	int *tile_map;

	int row;
	int col;

	int player_tile;
	entity * ent_player;

	Dict *dict_tile_map;
	Dict *save;

	save = Dict_New_Hash();
	if(!save) return;

	//save player
	ent_player = Entity_Get_Player();

	if(ent_player)
	{
		player_tile = tile_get_tile_number(player->position, player->boundBox);
		Dict_Hash_Insert(save, "PLAYER_TILE", Dict_New_int(player_tile));
	}

	//save tile info
	tile_map = Tile_Map_Get(tile_count, row, col); 
	if(tile_map)
	{
		dict_tile_map = Dict_New_Array( sizeof(int) );
		Dict_Array_Cpy(dict_tile_map, 0, tile_map, tile_count);
	
		//save tile dimensions and map
		Dict_Hash_Insert(save, "TILE_ROWS", Dict_New_int(row));
		Dict_Hash_Insert(save, "TILE_COLUMNS", Dict_New_int(col));
		Dict_Hash_Insert(save, "TILE_MAP", dict_tile_map);
	}

	//save
	save_dict_as_json(save, "save.def");
}

void Level_Editor_Move(SDL_Event *e)
{
	Vec2d move_cam;
	Vec2d new_cam_pos;

	Vec2dSet(move_cam, 0, 0);

	switch( e->key.keysym.sym )
    {
        case SDLK_UP:
			move_cam.y -= 5;
			break;
		case SDLK_DOWN:
			move_cam.y += 5;
			break;
		case SDLK_RIGHT:
			move_cam.x += 5;
			break;
		case SDLK_LEFT:
			move_cam.x -= 5;
			break;
		default:
			break;
	}
	
	Vec2dAdd(move_cam, Camera_Get_Camera(), new_cam_pos);
	Camera_SetPosition(new_cam_pos);
}