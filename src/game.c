#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
#include "sprite.h"
#include "entity.h"
#include "Tile.h"
#include "player.h"
#include "monster_ai.h"
#include "support_ai.h"
#include "items.h"
#include "structures.h"
#include "particle_emitter.h"

extern SDL_Surface *screen;
extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Rect Camera;
int current_time;
int last_time;
int delta;
void Init_All(); //single function to intialize all resource managers

/*this program must be run from the directory directly below images and src, not from within src*/
/*notice the default arguments for main.  SDL expects main to look like that, so don't change it*/
int main(int argc, char *argv[])
{
  SDL_Surface *temp = NULL;
  int done;
  int tx = 0,ty = 0;
  int i;
  const Uint8 *keys;
  char imagepath[512];
  SDL_Rect srcRect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
  SDL_Event e;
  last_time = current_time = SDL_GetTicks();
  
  Init_All();
  slog("Finished Init All()");
  done = 0;
  do
  {
	//render or draw functions go here
	//draw functions should go in order from background first to player draw calls last
    ResetBuffer();
    SDL_RenderClear(__gt_graphics_renderer);//clear screen

	tile_render();	
	player_draw();
	DrawMouse2();
	
	/*monster_spawn(Monster::grue);
	monster_spawn(Monster::spider01);
	monster_spawn(Monster::mino);		
	monster_spawn(Monster::orc);	
	support_spawn(Support::sara);	
	support_spawn(Support::healer);	
	support_spawn(Support::archer);
	
	*/
	entity_update_all();
	entity_think_all();
	entity_check_collision_all();
	particle_em_draw_all();
//	struct_update_all();
	

	G_MONSTER_SPAWN_TIMER -= 1;
    NextFrame();
	//end
    SDL_PumpEvents();
    keys = SDL_GetKeyboardState(NULL);
	//taken from lazyfoo
	//handles generally keyboard inputs	
	
	while( SDL_PollEvent( &e) != 0){
		if(e.type == SDL_QUIT)
		{
			done = 1;
		}
		else
		{
			player_move (&e);
		}
	}

		if(keys[SDL_SCANCODE_ESCAPE])
		{
			done = 1;
		}
		
	SDL_RenderPresent(__gt_graphics_renderer);
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
  sprite_initialize_system(1000); // allocates memory for all sprites
  entity_initialize_system();//allocate memory for all entities
  tile_init_system(); // allocate memory for tiles and generate map
  particle_em_init_system();
  player_init(); //creates player entity
  hud_init(); //loads hud sprites
//  structure_init_system();
  item_load_all(); //loads item imags into itemlist and allocates memory for new items
  inventory_init(); //allocates memory to store items
  InitMouse2(); // loads mouse sprite into mem
  

}
