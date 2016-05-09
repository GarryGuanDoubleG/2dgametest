#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "graphics.h"
#include "sprite.h"
#include "entity.h"
#include "player.h"
#include "audio.h"

#include "Tile.h"
#include "level.h"

#include "monster_ai.h"
#include "support_ai.h"

#include "items.h"
#include "structures.h"
#include "particle_emitter.h"
#include "camera.h"
#include "Menu.h"

extern SDL_Surface *screen;
extern SDL_Surface *buffer; /*pointer to the draw buffer*/

int current_time;
int last_time;
int delta;

void Init_All(); //single function to intialize all resource managers
void Draw_All();
/*this program must be run from the directory directly below images and src, not from within src*/
/*notice the default arguments for main.  SDL expects main to look like that, so don't change it*/
int main(int argc, char *argv[])
{
  int done;
  const Uint8 *keys;
  SDL_Event e;
  Music * music_new;
  last_time = current_time = SDL_GetTicks();
  
  Init_All();
  done = 0;

  music_new = Music_New("audio/bg_music.mp3", -1);

  if(!music_new)
  {
	  exit(1);
  }

  Music_Player(music_new);

  do
  {
	//render or draw functions go here
	//draw functions should go in order from background first to player draw calls last
    ResetBuffer();
    SDL_RenderClear(__gt_graphics_renderer);//clear screen

	Draw_All();

	monster_spawn(Monster::grue);
	
	monster_spawn(Monster::spider01);
	
	monster_spawn(Monster::mino);		
	/*
	monster_spawn(Monster::orc);	
	support_spawn(Support::sara);	
	support_spawn(Support::healer);	
	support_spawn(Support::archer);
	
	*/
	entity_update_all();
	entity_think_all();
	entity_check_collision_all();
//	struct_update_all();

	G_MONSTER_SPAWN_TIMER -= 1;
	//end
    SDL_PumpEvents();
    keys = SDL_GetKeyboardState(NULL);
	//taken from lazyfoo
	//handles generally keyboard inputs	
	
	while( SDL_PollEvent( &e) != 0)
	{
		if(e.type == SDL_QUIT)
		{
			done = 1;
		}
		else
		{
			Player_Move (&e);
		}
	}

	if(keys[SDL_SCANCODE_ESCAPE])
	{
		Level_Save();
		done = 1;
	}

	NextFrame();
	SDL_RenderPresent(Graphics_Get_Renderer());

	last_time = current_time;
	current_time = SDL_GetTicks();
	delta = current_time - last_time;
  }while(!done);
  exit(0);		/*technically this will end the program, but the compiler likes all functions that can return a value TO return a value*/
  return 0;
}
/*
* @brief Single function for calling all resource managers to statically allocate memory
*/
void Init_All()
{
	float bgcolor[] = {1,1,1,1};
  Init_Graphics(
	"Conquest",
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    bgcolor,
    0);
 
  srand(time(NULL));  //seed the random value at the start of the game
  
  if(TTF_Init() == -1)  
  {
	  exit(1);
  }

  Sprite_Initialize_System(1000); // allocates memory for all sprites
  entity_initialize_system();//allocate memory for all entities
  audio_init(128, 5); // allocates memory for audio channels
  particle_em_init_system();

  Menu_Title_Screen_Draw();
  Menu_Main_Draw();

  hud_init();

  inventory_init(); //allocates memory to store items
  item_load_all(); //loads item imags into itemlist and allocates memory for new items
  weapon_load_all();
  armor_load_all();
}

void Draw_All()
{
	tile_draw();	
	Player_Draw();
	Draw_Mouse();
	particle_em_draw_all();
}