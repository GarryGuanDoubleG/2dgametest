#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "dict.h"
#include "load.h"
#include "vector.h"
//#include <glib/glist.h>
#include <glib.h>

#define PARTICLE_MAX 10000
#define PARTICLE_EM_MAX 100

typedef enum particle_types
{
	PARTICLE_SPELLCAST
}Particle_Type;

typedef struct particle_S
{
	particle_types type;
	int nextFrame;//SDL_GetTicks start time
	int inuse;
	int frame;
	int scale;
	int rotation;
	int odd;
	Vec2d pos;
}Particle;

typedef struct Particle_Em_S
{
	int inuse;
	int id;
	int particle_count;
	GList *particle_list;
}Particle_Emitter;

void particle_em_init_system();
Particle_Emitter *particle_em_new();
Particle * particle_new(particle_types type, Vec2d pos, int scale, int rotation);
void particle_em_add(Particle_Emitter *p_em, particle_types type, Vec2d pos);
void particle_em_draw_all();
void particle_free(Particle * particle);
void particle_em_free(Particle_Emitter *p_em);


#endif