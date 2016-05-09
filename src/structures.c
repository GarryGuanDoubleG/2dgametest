#include "structures.h"

static int struct_show_select = false;
static entity * selected_struct = NULL;
extern struct Mouse;
extern entity *player;

void struct_update(entity *self)
{
	entity_draw(self);
}

Vec2d StructGetBuildPos()
{
	Vec2d player_pos;
	Vec2d build_pos;
	Vec2d offset;

	player_pos = player->position;

	if(player->face_dir == 0)		
		Vec2dSet(offset, 0, -128);
	else if(player->face_dir == 1)
		Vec2dSet(offset, -128, 0);
	else if(player->face_dir == 2)
		Vec2dSet(offset, 0, 128);
	else if(player->face_dir == 3)
		Vec2dSet(offset, 128, 0);

	Vec2dAdd(offset, player->position, build_pos);

	return build_pos;
}

void struct_touch(entity *self, entity *other)
{
	Vec2d new_pos;
	Vec2d neg_vel;

	VectorNegate2d(other->velocity, neg_vel);

	Vec2dAdd(other->position, neg_vel, other->position);

	Vec2dSet(other->velocity, 0, 0);
}

entity *struct_wall_spawn()
{
	Sprite *sprite;
	cStruct * wall;

	Vec2d build_pos;
	int build_tile;
	int tile_type;

	build_pos = StructGetBuildPos();

	build_tile = tile_get_tile_number(build_pos, New_SDL_Rect(0, 0, 1, 1));
	tile_type = tile_get_type(build_tile);

	if(tile_type != TILE_GRASS && tile_type != TILE_ROAD)
	{
		slog("Can't build there");
		return NULL;
	}

	build_pos = tile_get_pos(build_tile);

	sprite = Sprite_Load("images/structures/wall_01.png", 66, 160, 100, 120);
	sprite->fpl = 1;

	wall = entity_load(sprite, build_pos, 1000, 0, 0);
	wall->type = ENTITY_TYPE::WALL;

	wall->boundBox = New_SDL_Rect( 10, 10, 110, 110);

	wall->update = struct_update;
	wall->touch = struct_touch;
	wall->frame = 0;

	return wall;
}

entity *structure_spawn(int type)
{
	if(type == ENTITY_TYPE::WALL)
	{
		return struct_wall_spawn();	
	}
}


void update_selected_struct(entity *self)
{
	Vec2d m_pos = Get_Mouse_Pos();
	Vec2d cam_offset = {Camera_Get_Camera().x, Camera_Get_Camera().y } ;
	Vec2d frame_offset;
	Vec2d draw_pos; 

	if(!self)
	{
		slog("No struct self");
		return;
	}
	if(self->selected)
	{
		frame_offset.x = self->sprite->frameW/2;
		frame_offset.y = self->sprite->frameH/2;

		Vec2dAdd(m_pos,cam_offset, draw_pos);
		Vec2dSubtract(draw_pos, frame_offset, draw_pos); 
		self->boundBox.x = draw_pos.x;
		self->boundBox.y = draw_pos.y;
		self->position = draw_pos;

		entity_draw(self);
	}	
}

//puts structure into the world
int structure_place()
{
	if(!selected_struct)
	{
		slog("Select Struct is NULL. Can't Place");
		return false;
	}

	slog("placing structure");
	selected_struct->selected = false;
	if(!tile_collision(selected_struct->position, selected_struct->boundBox)
	   && !entity_check_collision(selected_struct))
	{
		selected_struct->placed = true;
		selected_struct->update = struct_update;
		//tile_make_bound(selected_struct->boundBox);
		selected_struct = NULL;
		return true;
	}
	slog("finished placing structure");
	selected_struct = NULL;
	return false;
}
