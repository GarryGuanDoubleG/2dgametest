#include "structures.h"

static int struct_show_select = false;
static entity * selected_struct = NULL;
extern struct Mouse;
/*
structure * struct_load(Sprite *sprite, int health, int defense, int type)
{
	int i;
	for(i = 0; i < STRUCT_MAX; i++)
	{
		if(struct_list[i].inuse == false)
		{
			struct_list[i].inuse = true;
			struct_list[i].sprite = sprite;
			struct_list[i].health = health;
			struct_list[i].defense = defense;
		}
		return &struct_list[i];
	}

	slog("Could not load structure");
	return NULL;
}
*/
void struct_update(entity *self)
{
	entity_draw(self, self->position.x, self->position.y);
}

void update_selected_struct(entity *self)
{
	Vec2d m_pos = Get_Mouse_Pos();
	Vec2d cam_offset = {Graphics_Get_Player_Cam().x, Graphics_Get_Player_Cam().y } ;
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

		entity_draw(self, draw_pos.x, draw_pos.y);
	}	
}
/*
void struct_update_all()
{
	int i;
	int j;
	for(i = 0; i < STRUCT_MAX; i++)
	{
		if(struct_list[i].inuse == false)
		{
			continue;
		}
		if(!struct_list[i].update)
		{
			continue;
		}
		struct_list[i].update(&struct_list[i]);

		if(struct_list[i].placed == true)
		{
			structure_draw(&struct_list[i], struct_list[i].position);
			for(j = 0; j < ENTITY_MAX; j++)
			{
				if(!entityList[i].inuse)
				{
					continue;
				}
				if(struct_collision(&struct_list[i], &entityList[i]))
				{
					entityList[i].position.x -= entityList[i].velocity.x;
					entityList[i].position.y -= entityList[i].velocity.y;
				}
			}
		}
	}
}
*/


void structure_select(int type)
{
	slog("structure select");
	if(selected_struct)
	{
		entity_free(selected_struct);
		selected_struct = NULL;
		return;
	}
	if(type == struct_type::main_base01)
	{
		Sprite *struct_sprite = Sprite_Load(SPRITE_MAIN_BASE01_FILEPATH, SPRITE_IMAGE_W, SPRITE_IMAGE_H, SPRITE_FRAME_W, SPRITE_FRAME_H);
		selected_struct = struct_load(struct_sprite, 1000, 100, type);
	}
	if(type == struct_type::wall_01)
	{
		Sprite *struct_sprite = Sprite_Load(SPRITE_WALL01_FILEPATH, SPRITE_IMAGE_W, SPRITE_IMAGE_H, SPRITE_FRAME_W, SPRITE_FRAME_H);
		selected_struct = struct_load(struct_sprite, 1000, 100, type);
	}
	if(type == struct_type::tower_01)
	{
		Sprite *struct_sprite = Sprite_Load(SPRITE_WALL01_FILEPATH, SPRITE_IMAGE_W, SPRITE_IMAGE_H, SPRITE_FRAME_W, SPRITE_FRAME_H);
		selected_struct = struct_load(struct_sprite, 1000, 100, type);
	}

	selected_struct->boundBox.x = 0;
	selected_struct->boundBox.y = 0;
	selected_struct->boundBox.w = selected_struct->sprite->frameW;
	selected_struct->boundBox.h = selected_struct->sprite->frameH;
	selected_struct->update = update_selected_struct;
	selected_struct->selected = true;

	slog("finished selecting structure");
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
