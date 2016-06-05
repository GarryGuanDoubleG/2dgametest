#include "particle_emitter.h"
#include "sprite.h"
#include "graphics.h"
#include <math.h>
#include <random>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//#include <glib/ghash.h>
#include <SDL.h>

GHashTable *__particle_sprite_hash = NULL;
Particle_Emitter *__particle_em_list = NULL;
Particle *__particle_list = NULL;
int __particle_em_count = 0;
int __particle_count = 0;

void particle_load_from_def(char *filename);
void particle_close_system();
extern int delta;

Sprite * particle_get_sprite(particle_types type)
{
	Sprite* sprite;
	char * key;
	switch(type)
	{
		case PARTICLE_SPELLCAST:
			key = g_strdup("spellcast");
			sprite = (Sprite*) g_hash_table_lookup(__particle_sprite_hash, key);
			return sprite;
		default:
			return NULL;
	}
}

void particle_close_system()
{
	int i;
	
	for(i = 0; i < __particle_em_count; i++)
	{
		g_list_free(__particle_em_list[i].particle_list);
	}
	
	memset(__particle_em_list, 0, sizeof(Particle_Emitter) * PARTICLE_EM_MAX);
	memset(__particle_list, 0, sizeof(Particle) * PARTICLE_MAX);
	g_hash_table_destroy(__particle_sprite_hash);

	__particle_em_list = NULL;
	__particle_list = NULL;
	__particle_sprite_hash = NULL;
}

void particle_em_init_system()
{
	__particle_sprite_hash = g_hash_table_new_full(g_str_hash,
							  g_str_equal,
							  (GDestroyNotify)dict_g_string_free,
							  (GDestroyNotify)NULL);
	__particle_list = (Particle *)malloc(sizeof(Particle) * PARTICLE_MAX);
	memset(__particle_list, 0, sizeof(Particle) * PARTICLE_MAX);

	__particle_em_list = (Particle_Emitter *) malloc(sizeof(Particle_Emitter) * PARTICLE_EM_MAX);
	memset(__particle_em_list, 0, sizeof(Particle_Emitter *) * PARTICLE_EM_MAX);

	__particle_em_count = 0;
	__particle_count = 0;

	atexit(particle_close_system);

	particle_load_from_def("def/particle.def");
}
Particle_Emitter * particle_em_new()
{
	Particle_Emitter *p_em = NULL;
	int i;
	for(i = 0; i <= __particle_em_count; i++)
	{
		if(__particle_em_list[i].inuse)
			continue;
		++__particle_em_count;
		p_em = &__particle_em_list[i];
		p_em->inuse = Bool_True;
		p_em->id = i;
		p_em->particle_count = 0;
		p_em->particle_list = NULL;
		break;
	}

	return p_em;
}
Particle * particle_new(particle_types type, Vec2d pos, int scale, int rotation)
{
	Particle *particle;
	int i;
	for(i = 0; i <= __particle_count; i++)
	{
		if(__particle_list[i].inuse)
			continue;
		++__particle_count;
		particle = &__particle_list[i];
		particle->inuse = Bool_True;
		particle->type = type;
		particle->pos = pos;
		particle->scale = scale;
		particle->rotation = rotation;
		particle->frame = 0;
		particle->nextFrame = 150;
		break;
	}
	return particle;
}

void particle_free(Particle *particle)
{
	particle->inuse = false;
	memset(particle, 0, sizeof(Particle));
}

void particle_em_free(Particle_Emitter *p_em)
{
	g_list_free(p_em->particle_list);
	memset(p_em, 0, sizeof(Particle_Emitter));
}

void particle_load_from_def(char *filename)
{
	Dict *particle_dict;
	int i = 0;
	Line sprite_key;
	Line key;
	char * filepath;
	Sprite *particle_sprite, *temp;
	int  img_width,img_height, frame_width, frame_height, fpl;

	particle_dict = load_dict_from_file(filename);

	if(particle_dict->data_type != DICT_HASH)
	{
		slog("Error: file %s should be json hash ");
	}

	for(i = 0; i < particle_dict->item_count; i++)
	{
		Dict *value = dict_get_hash_nth(key, particle_dict, i);
		if(value->data_type != DICT_HASH)
		{
			slog("Error: %s should be a hash", key);
			break;
		}
		filepath = (char *)(dict_get_hash_value(value, "filepath")->keyValue);
		slog("particle sprite filepath: %s", filepath);
		img_width = atoi((char *)(dict_get_hash_value(value, "img_width")->keyValue));
		img_height = atoi((char *) (dict_get_hash_value(value, "img_height")->keyValue));
		frame_width = atoi((char*)(dict_get_hash_value(value, "frame_width")->keyValue));
		frame_height = atoi((char *) (dict_get_hash_value(value, "frame_height")->keyValue));
		fpl = atoi((char *) (dict_get_hash_value(value, "frames_per_line")->keyValue));
		slog("particle img width height: %i %i", img_width, img_height);
		slog("particle frame width height: %i %i", frame_width, frame_height);
		particle_sprite = Sprite_Load(filepath, img_width, img_height, frame_width, frame_height);
		particle_sprite->fpl = fpl;
		if(particle_sprite)
		{
			g_hash_table_insert(__particle_sprite_hash, g_strdup(key), particle_sprite);
		}
	}
}

void particle_em_add(Particle_Emitter *p_em, particle_types type, Vec2d pos)
{
	Particle * p1, *p2, *p3;
	Sprite * sprite;
	Vec2d p;
	int ran_num = rand();
	if(!p_em)
	{
		slog("Particle or Particle Emitter NULL");
		return;
	}
	sprite = particle_get_sprite(type);
	if(!sprite)
	{
		return;
	}

	p1 = particle_new(type, pos, 0, 0);
	p1->odd = false;
	p1->pos.y = p1->pos.y + rand() % 16 - 7;
	p2 = particle_new(type, pos, 0, 0);
	p2->odd = true;
	p2->pos.x += ran_num % (int)(sprite->frameW/2);
	p2->pos.y = p2->pos.y + rand() % 16 - 7;
	p3 = particle_new(type, pos, 0,0);
	p3->odd = false;
	p3->pos.x += (int)(sprite->frameW/1.6);
	p3->pos.y = p3->pos.y + rand() % 16 - 7;

	p_em->particle_list = g_list_insert(p_em->particle_list, p1,p_em->particle_count);
	p_em->particle_list = g_list_insert(p_em->particle_list, p2,p_em->particle_count);
	p_em->particle_list = g_list_insert(p_em->particle_list, p3,p_em->particle_count);
	p_em->particle_count++;
}
void particle_em_clean(Particle_Emitter *p_em)
{
	int i = 0;
	gpointer elem;
	Sprite *sprite;
	Particle *particle;
	GFOREACH(elem, p_em->particle_list)
	{
		particle = (Particle *) elem;
		if(!elem || !particle)
		{
			break;
		}

		if(particle)
		{
			sprite = particle_get_sprite(particle->type);
			if(particle->frame > sprite->fpl)
			{
				p_em->particle_list = g_list_remove(p_em->particle_list, particle);
				particle_free(particle);
				p_em->particle_count--;
			}
		}
	}
}


void particle_think(void *data, void *user_data)
{
}
void particle_draw(void *data, void *user_data)
{
	Particle * particle = (Particle*) data;
	Sprite * p_sprite;
	if(!particle || !data) return;
	p_sprite = particle_get_sprite(particle->type);
	if(p_sprite == NULL)
	{
		slog("Sprite is NULL");
	}
	particle->pos.y -= 150 * delta / 1000;
	particle->nextFrame -= delta;
	if(particle->nextFrame <= 0)
	{
		particle->frame++;
		particle->nextFrame = 150;
	}
	Sprite_Draw( p_sprite, particle->frame, particle->pos);
}

void particle_em_draw(Particle_Emitter *p_em)
{
	if(!p_em) return;
	if(!p_em->particle_list) return;
	g_list_foreach(p_em->particle_list, particle_draw, NULL);
	particle_em_clean(p_em);
}
void particle_em_draw_all()
{
	int i = 0;
	for(i = 0; i < __particle_em_count; i++)
	{
		if(!__particle_em_list[i].inuse)
		{
			continue;
		}
		particle_em_draw(&__particle_em_list[i]);
	}
}