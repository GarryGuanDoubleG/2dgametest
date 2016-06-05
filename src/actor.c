#include "json_parse.h"

#include "load.h"
#include "save.h"

#include "actor.h"
#include "camera.h"

static Actor		*g_actor_list = NULL;
static int			g_actor_count;
static int			g_actor_max;

void actor_empty_list()
{
	int i;

	if(!g_actor_list)
	{
		return;
	}

	for(i = 0; i < g_actor_max; i++)
	{
		Actor *actor;

		if(!g_actor_list[i].active)
		{
			continue;
		}

		actor = &g_actor_list[i];
		actor_free(&actor);
	}

	memset(g_actor_list, 0, sizeof(Actor) * g_actor_max);

	g_actor_count = 0;

}

void actor_free(Actor **self)
{
	Actor *target;

	if(!self || !target)
	{
		slog("Actor is null");
		return;
	}

	target = *self;
	target->active = Bool_False;

	if(target->sprite)
	{
		Sprite_Free(&target->sprite);
	}

	g_actor_count--;
	*self = NULL;
}

void actor_close_system()
{
	int i;
	Actor *actor;

	if(!g_actor_list)
	{
		return;
	}

	for(i = 0; i < g_actor_max; i++)
	{

		actor = &g_actor_list[i];
		actor_free(&actor);
	}

	g_actor_count = 0;
	free(g_actor_list);
	g_actor_list = NULL;
}

void actor_initialize_system(int actor_max)
{
	int i;

	if(actor_max == 0)
	{
		slog("Maximum actors cannot be 0");
	}

	g_actor_count = 0;
	g_actor_max   = actor_max;
	g_actor_list  = (Actor*)malloc(sizeof(Actor) * actor_max);

	if(!g_actor_list)
	{
		slog("Failed to initialize actor system");
		return;
	}

	memset(g_actor_list, 0, sizeof(Actor) * actor_max);

	for(i = 0; i < actor_max; i++)
	{
		g_actor_list[i].sprite = NULL;
	}

	atexit(actor_close_system);
}

Actor *actor_new(Vec2d position, Uint8 type)
{
	int i;

	if(g_actor_count > g_actor_max)
	{
		slog("Hit max Actors. Can not make more");
		return NULL;
	}

	for(i = 0; i < g_actor_max; i++)
	{
		if(g_actor_list[i].active)
		{
			continue;
		}

		memset(&g_actor_list[i], 0, sizeof(Actor));

		g_actor_list[i].active = Bool_True;
		g_actor_list[i].type = type;
		g_actor_list[i].position = position;
		
		g_actor_list[i].draw = &Sprite_Draw;
	}

	return NULL;
}