#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "sprite.h"
#include "entity.h"

extern entity *player;
extern char * player_char_file;
extern const int PLAYER_HEIGHT;
extern const int PLAYER_WIDTH;

void player_init();
void player_draw();
<<<<<<< HEAD
void player_move(SDL_Event *e);
void player_think();

=======
void player_think(entity *self);
void player_move(SDL_Event e);
>>>>>>> f938880194512dc5de165eac4f3db7c7e5ae0628


#endif