#include "structures.h"


structure * struct_list;
static int struct_show_select = false;
static structure * selected_struct = NULL;
extern struct Mouse2;

void structure_init_system()
{
	int i;

	struct_list = (structure*)malloc(sizeof(structure) * STRUCT_MAX);
	memset(struct_list, 0, sizeof(structure) * STRUCT_MAX);

	for(i = 0; i < STRUCT_MAX; i++)
	{
		struct_list[i].sprite = NULL;
		struct_list[i].inuse = false;
	}
	atexit(structure_close_system);
}

structure * struct_load(Sprite2 *sprite, int health, int defense, int type)
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

void structure_free(structure * self)
{
	self->sprite = NULL;
	self->inuse = false;
	memset(self, 0, sizeof(structure));
}

void structure_close_system()
{
	int i;
	for(i = 0; i < STRUCT_MAX; i++)
	{
		if(struct_list[i].inuse == true)
		{
//			structure_free( &struct_list[i]);
		}
	}

	memset(struct_list, 0, sizeof(struct_list) * STRUCT_MAX);
	free(struct_list);
}

void structure_draw(structure *self, Vec2d draw_pos)
{
	if(!self)
	{
		slog("Cannot draw Null structure");
		return;
	}

	slog("Draw pos X%f Y%f", draw_pos.x, draw_pos.y);
	sprite_draw(self->sprite, 0, 0, graphics_get_renderer(), draw_pos.x, draw_pos.y);
}

void update_selected_struct(structure *self)
{
	Vec2d m_pos = get_mouse_pos();
	Vec2d cam_offset = {graphics_get_player_cam().x, graphics_get_player_cam().y } ;
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
		Vec2dSub(draw_pos, frame_offset, draw_pos); 
		self->boundBox.x = draw_pos.x;
		self->boundBox.y = draw_pos.y;
		self->position = draw_pos;
		structure_draw(self, draw_pos);
	}
}

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

void structure_select(int type)
{
	if(selected_struct)
	{
		structure_free(selected_struct);
		selected_struct = NULL;
		return;
	}

	if(type == struct_type::main_base01)
	{
		Sprite2 *struct_sprite = sprite_load(SPRITE_MAIN_BASE01_FILEPATH, SPRITE_IMAGE_W, SPRITE_IMAGE_H, SPRITE_FRAME_W, SPRITE_FRAME_H);
		selected_struct = struct_load(struct_sprite, 1000, 100, type);
	}
	if(type == struct_type::wall_01)
	{
		Sprite2 *struct_sprite = sprite_load(SPRITE_WALL01_FILEPATH, SPRITE_IMAGE_W, SPRITE_IMAGE_H, SPRITE_FRAME_W, SPRITE_FRAME_H);
		selected_struct = struct_load(struct_sprite, 1000, 100, type);
	}
	if(type == struct_type::tower_01)
	{
		Sprite2 *struct_sprite = sprite_load(SPRITE_WALL01_FILEPATH, SPRITE_IMAGE_W, SPRITE_IMAGE_H, SPRITE_FRAME_W, SPRITE_FRAME_H);
		selected_struct = struct_load(struct_sprite, 1000, 100, type);
	}

	selected_struct->boundBox.x = 0;
	selected_struct->boundBox.y = 0;
	selected_struct->boundBox.w = selected_struct->sprite->frameW;
	selected_struct->boundBox.h = selected_struct->sprite->frameH;
	selected_struct->update = update_selected_struct;
	selected_struct->selected = true;
}
//puts structure into the world
int structure_place()
{
	if(!selected_struct)
	{
		slog("Select Struct is NULL. Can't Place");
		return false;
	}

	selected_struct->selected = false;
	if(!tile_structure_collision(selected_struct->boundBox))// && !entity_structure_collision(selected_struct->boundBox))
	{
		selected_struct->placed = true;
		//tile_make_bound(selected_struct->boundBox);
		selected_struct = NULL;
		return true;
	}
	selected_struct = NULL;
	return false;
}

int struct_collision(structure *self, entity *other)
{
	SDL_Rect ent_rect = {other->position.x + other->boundBox.x, other->position.y + other->boundBox.y,
						other->boundBox.w, other->boundBox.h};
	return rect_collide(self->boundBox, ent_rect);
}