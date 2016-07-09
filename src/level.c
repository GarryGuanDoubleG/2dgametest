#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "level.h"
#include "load.h"
#include "save.h"
#include "simple_logger.h"
#include "json_parse.h"
#include "graphics.h"

static Vec2d G_em_camera_bound;
static Vec2d G_Mouse_Pos;

GHashTable *tile_hash;
GHashTable *tile_type;
GHashTable *tile_pos_hash;

int G_Selected_Type = -1;
int G_Store_Pos = Bool_True;
int count = 0;

extern Entity *g_player;

void Level_Editor_Input(SDL_Event *e);

void Level_Set_Select(gpointer key, gpointer tile_pos, gpointer data)
{
	SDL_Rect click_box;
	SDL_Rect tool_box;

	Vec2d *tool_pos;
	Vec2d *click_pos;
	Sprite *tile;
		
	char * tile_key;

	tile_key = g_strdup( (char *)key );

	tool_pos = (Vec2d *) tile_pos;
	click_pos = (Vec2d *) data;
	
	click_box = New_SDL_Rect(click_pos->x, click_pos->y, 16, 16);
	tool_box = New_SDL_Rect(tool_pos->x, tool_pos->y, TILE_WIDTH, TILE_HEIGHT);

	if(rect_collide(tool_box, click_box))
	{
		G_Selected_Type = atoi( (char *)g_hash_table_lookup(tile_type, key));
	}
}

void Level_Tool_Select()
{
	Vec2d click_pos;
	Vec2d tile_pos;
	int i;

	click_pos = G_Mouse_Pos;
	Vec2dAdd(click_pos, Camera_Get_Editor_Offset(), click_pos);

	g_hash_table_foreach(tile_pos_hash, Level_Set_Select, &click_pos);

	click_pos = G_Mouse_Pos;
	if( G_Mouse_Pos.x > 50 && click_pos.x < 256
		&& click_pos.y > 250 && click_pos.y < 450)
	{
		slog("forest gen");
		G_Selected_Type = -1;

		for(i = 0; i < TOTAL_TILES; i++)
		{
			tile_list[i].mType = TILE_TREE;
			dest_tile_list[i].mType = TILE_TREE;
		}
	}

	if( G_Mouse_Pos.x > 50 && click_pos.x < 256
		&& click_pos.y > 480 && click_pos.y < 680)
	{
		slog("path gen");
		G_Selected_Type = -1;
		tile_forest_gen();
	}
}

void Level_Editor_Load_Assets()
{
	Dict *assets;
	Dict *value;
	int i, j;
	Line key;

	assets = Load_Dict_From_File("editor/editor_asset.def");

	//get tile rows
	value = Dict_Get_Hash_Value(assets, "terrain");
	if(!value || value->data_type != DICT_HASH) return;
	
	for(i = 0; i < value->item_count; i++)
	{
		Sprite *sprite;
		char *filepath;

		int *imageW, *imageH;
		int *frameW, *frameH;
		char * type;

		Dict * sprite_info;
		sprite_info = dict_get_hash_nth(key, value, i);

		if(!sprite_info) return;

		filepath = (char *)(Dict_Get_Hash_Value(sprite_info, "image")->keyValue);

		imageW = (int *)(Dict_Get_Hash_Value(sprite_info, "imageW")->keyValue);
		imageH = (int *)(Dict_Get_Hash_Value(sprite_info, "imageH")->keyValue);
		frameW = (int *)(Dict_Get_Hash_Value(sprite_info, "frameW")->keyValue);
		frameH = (int *)(Dict_Get_Hash_Value(sprite_info, "frameH")->keyValue);
		type = (char*)(Dict_Get_Hash_Value(sprite_info, "type")->keyValue);

		sprite = Sprite_Load(filepath, *imageW, *imageH, *frameW, *frameH);
		
		if(!sprite) return;

		g_hash_table_insert(tile_hash, g_strdup(key), sprite);
		g_hash_table_insert(tile_type, g_strdup (key), g_strdup(type));
	}
}

void Level_Editor_Init()
{
	Vec2d em_cam_size;
	Vec2d em_cam_offset;

	int cam_width = 1080;
	int cam_height = 640;

	//offset camera to left
	int cam_pos_x = 300;
	int cam_pos_y = 50;

	Vec2dSet(em_cam_size, cam_width, cam_height);
	Vec2dSet(em_cam_offset, cam_pos_x, cam_pos_y);
	Vec2dCopy(em_cam_offset, G_em_camera_bound);

	Camera_SetSize(em_cam_size);
	Camera_SetPosition(em_cam_offset);
	Camera_Set_Editor_Offset(em_cam_offset);

	tile_init_system(MODE_EDITOR);

	tile_hash = g_hash_table_new_full(g_str_hash,
							  g_str_equal,
							  (GDestroyNotify)dict_g_string_free,
							  (GDestroyNotify)dict_destroy);

	tile_type = g_hash_table_new_full(g_str_hash,
							  g_str_equal,
							  (GDestroyNotify)dict_g_string_free,
							  (GDestroyNotify)dict_destroy);

	tile_pos_hash = g_hash_table_new_full(g_str_hash,
							  g_str_equal,
							  (GDestroyNotify)dict_g_string_free,
							  (GDestroyNotify)dict_destroy);

	Level_Editor_Load_Assets();
}

void Level_Tile_Tools_Draw(void *key, gpointer sprite, gpointer pos)
{
	Vec2d *curr_pos;
	Vec2d *next_pos;
	Vec2d *tool_pos;

	Vec2d draw_pos;
	Vec2d camera_pos;

	char * tile_key;
	Sprite *tile_sprite;

	int type;
	
	curr_pos = (Vec2d *)pos;
	next_pos = (Vec2d *)pos;

	tile_key = (char *)key;

	type = atoi( (char *) g_hash_table_lookup(tile_type, tile_key));

	tile_sprite = (Sprite *)sprite;

	camera_pos = Camera_GetPosition();

	Vec2dSet(draw_pos, curr_pos->x, curr_pos->y);
	Vec2dAdd(draw_pos, camera_pos, draw_pos);

	if(type < 0)
	{
		if(!strcmp(tile_key, "forest_gen"))
		{			
			draw_pos.x = 330 + camera_pos.x;
			draw_pos.y = 300 + camera_pos.y;

			Vec2dSubtract(Camera_Get_Editor_Offset(), draw_pos, draw_pos);
		}
		else
		{
			draw_pos.x = 330 + camera_pos.x;
			draw_pos.y = 500 + camera_pos.y;;

			Vec2dSubtract(Camera_Get_Editor_Offset(), draw_pos, draw_pos);
		}
		Sprite_Draw((Sprite *)sprite, 0, draw_pos);
		return;
	}

	if(G_Store_Pos == Bool_True)
	{
		tool_pos = (Vec2d *)malloc(sizeof(Vec2d));

		tool_pos->x = draw_pos.x;
		tool_pos->y = draw_pos.y;

		g_hash_table_insert(tile_pos_hash, g_strdup( tile_key ), tool_pos);
	}

	if(G_Selected_Type != type)
	{
		SDL_SetTextureColorMod(tile_sprite->image, 100,100,200);
		Sprite_Draw(tile_sprite, 0, draw_pos);
		SDL_SetTextureColorMod(tile_sprite->image, 255,255,255);
	}
	else
	{
		Sprite_Draw(tile_sprite, 0, draw_pos);
	}
	//set next tile to draw to the right
	next_pos->x += tile_sprite->frameW + 10;

	//if tile is too far right, start on new row
	if(next_pos->x > TILE_WIDTH * 2)
	{
		next_pos->x = 0;
		next_pos->y += TILE_HEIGHT;
	}
}

void Level_Editor_Draw()
{
	Vec2d asset_draw_pos;

	Vec2dSet(asset_draw_pos, 0, 0);

    SDL_RenderClear(Graphics_Get_Renderer());//clear screen

	//draw tile assets
	g_hash_table_foreach(tile_hash, Level_Tile_Tools_Draw, &asset_draw_pos);
	//stored tile asset positions
	G_Store_Pos = Bool_False;

	tile_draw();
	Draw_Mouse();

	graphics_next_frame();
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
				Level_Editor_Input(&e);
			}
		}

		if(keys[SDL_SCANCODE_ESCAPE])
		{
			Level_Save();
			SDL_PumpEvents();
			SDL_RenderClear(Graphics_Get_Renderer());
			done = 1;
		}
	}while(!done);
}

void Level_Load_New()
{
  tile_init_system(Bool_True); // allocate memory for tiles and generate map
  Player_Init(); //creates player Entity
}

void Level_Load(int load)
{
	Dict * last_save;
	Dict * value;

	int *tile_map;
	int *tile_row;
	int *tile_col;

	if(load == Bool_False)
	{
		Level_Load_New();
		return;
	}

	last_save = Load_Dict_From_File("save.def");
	
	//get tile rows
	value = Dict_Get_Hash_Value(last_save, "TILE_ROWS");
	if(!value) return;
	tile_row = (int *)value->keyValue;

	//get tile columns
	value = Dict_Get_Hash_Value(last_save, "TILE_COLUMNS");
	if(!value) return;
	tile_col = (int *)value->keyValue;

	//get map
	value = Dict_Get_Hash_Value(last_save, "TILE_MAP");
	if(!value) return; 
	tile_map = (int *)value->keyValue;

	//load map into game
	tile_load_from_def(tile_map); // allocate memory for tiles and generate map

	//player data should be stored in a hash
	//but only starting tile is being saved;
	value = Dict_Get_Hash_Value(last_save, "PLAYER_TILE");
	if(!value)
		Player_Load_from_Def(NULL);
	 else
		Player_Load_from_Def(value); //creates player Entity
}

void Level_Save()
{
	int i;

	int tile_count;
	int *tile_map;

	int row;
	int col;

	int player_tile;
	
	Entity * ent;

	Dict *dict_tile_map;
	Dict *Entity_save_hash;
	Dict *value;
	Dict *save;

	save = Dict_New_Hash();
	Entity_save_hash = Dict_New_Hash();
	value = Dict_New_Hash();
	if(!save) return;

	//save player
	g_player;

	if(g_player)
	{
		player_tile = Tile_Get_Index(g_player->position, g_player->boundBox);
		Dict_Hash_Insert(save, "PLAYER_TILE", Dict_New_Int(player_tile));
	}

	//save tile info
	tile_map = Tile_Map_Get(tile_count, row, col); 
	if(tile_map)
	{
		dict_tile_map = Dict_New_Array( sizeof(int) );
		Dict_Array_Cpy(dict_tile_map, 0, tile_map, tile_count);
	
		//save tile dimensions and map
		Dict_Hash_Insert(save, "TILE_ROWS", Dict_New_Int(row));
		Dict_Hash_Insert(save, "TILE_COLUMNS", Dict_New_Int(col));
		Dict_Hash_Insert(save, "TILE_MAP", dict_tile_map);
	}

	//save
	save_dict_as_json(save, "save.def");
}

void Level_Editor_Input(SDL_Event *e)
{
	int tile_index;

	Vec2d move_cam_offset;
	Vec2d new_cam_pos;

	Vec2d tile_pos;
	Vec2d tile_map_pos;
	Vec2d mouse_pos;

	Vec2d camera;
	Vec2d cam_offset;

	SDL_Rect mouse_bounds;
	SDL_Rect tile_map_bounds;
	SDL_Rect camera_bounds;

	Vec2dSet(move_cam_offset, 0, 0);

	camera = Camera_GetPosition();
	cam_offset = Camera_Get_Editor_Offset();
	camera_bounds = Camera_Get_Camera();

	Vec2dAdd(cam_offset, camera, tile_map_pos);

	switch( e->key.keysym.sym )
    {
        case SDLK_UP:
			move_cam_offset.y -= TILE_HEIGHT;
			break;
		case SDLK_DOWN:
			move_cam_offset.y += TILE_HEIGHT;
			break;
		case SDLK_RIGHT:
			move_cam_offset.x += TILE_WIDTH;
			break;
		case SDLK_LEFT:
			move_cam_offset.x -= TILE_WIDTH;
			break;
		default:
			break;
	}
	
	if(e->type == SDL_MOUSEBUTTONDOWN)
	{
		G_Mouse_Pos = Get_Mouse_Pos();
		
		mouse_pos = G_Mouse_Pos;
		Vec2dAdd(mouse_pos, camera, mouse_pos);
		Vec2dSubtract(cam_offset, mouse_pos, mouse_pos);
		mouse_bounds = New_SDL_Rect(mouse_pos.x, mouse_pos.y, 16, 16);

		slog("mouse pos %f %f", mouse_pos.x, mouse_pos.y);
		slog("cam x y %f %f", camera.x, camera.y);

		tile_map_bounds = New_SDL_Rect(camera.x, camera.y, camera_bounds.w, camera_bounds.h);

		//need to edit for readability
		if(rect_collide(mouse_bounds, tile_map_bounds) && G_Selected_Type >= 0 )
		{	
			Vec2dSubtract(cam_offset, mouse_pos, mouse_pos);
			
			mouse_bounds = New_SDL_Rect(0, 0, 16, 16);
			//gets tile number based on original absolute positions not relative to camera
			//subtract by offset
			tile_index = Tile_Get_Index(mouse_pos, mouse_bounds);
			tile_editor_set_type(tile_index, G_Selected_Type);
			slog("clicked tile index %i", tile_index);
		}

		Level_Tool_Select();
	}

	Vec2dAdd(move_cam_offset, Camera_Get_Camera(), new_cam_pos);

	if(new_cam_pos.x < G_em_camera_bound.x || 
	   new_cam_pos.y < G_em_camera_bound.y)
	{
	   return;
	}

	Camera_SetPosition(new_cam_pos);
}