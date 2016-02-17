#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
#include "sprite.h"
#include "Tile.h"
#include "player.h"

extern SDL_Surface *screen;
extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Rect Camera;

void Init_All();

int getImagePathFromFile(char *filepath,char * filename);
int getCoordinatesFromFile(int *x, int *y,char * filename);
void addCoordinateToFile(char *filepath,int x, int y);


/*this program must be run from the directory directly below images and src, not from within src*/
/*notice the default arguments for main.  SDL expects main to look like that, so don't change it*/
int main(int argc, char *argv[])
{
  SDL_Surface *temp = NULL;
  int done;
  int tx = 0,ty = 0;
  const Uint8 *keys;
  char imagepath[512];
  SDL_Rect srcRect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
  SDL_Event e;

  Init_All();
  /*temp = IMG_Load("images/bgtest.png");/*notice that the path is part of the filename*/
  /*if(temp != NULL)
  {
      printf("temp image loaded successfully\n");
      SDL_BlitSurface(temp,NULL,buffer,NULL);
  }
  gt_graphics_render_surface_to_screen(temp,srcRect,0,0);
  SDL_FreeSurface(temp);*/
  done = 0;
  do
  {
    ResetBuffer();
	SDL_RenderClear(__gt_graphics_renderer);//clear screen
	//render or draw functions go here
	//draw functions should go in order from background first to player draw calls last
	tile_render(NULL);
    DrawMouse2();
	player_draw();
	while(SDL_PollEvent(&e) != 0)
		player_move (&e);
    NextFrame();
	//end
    SDL_PumpEvents();
    keys = SDL_GetKeyboardState(NULL);
    if(keys[SDL_SCANCODE_ESCAPE])
    {
        done = 1;
    }

	SDL_RenderPresent(__gt_graphics_renderer);
  }while(!done);
  exit(0);		/*technically this will end the program, but the compiler likes all functions that can return a value TO return a value*/
  return 0;
}

void CleanUpAll()
{
  CloseSprites();
  /*any other cleanup functions can be added here*/ 
}

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
  //GG edit
  sprite_initialize_system(1000); // allocates memory for all sprites
  entity_initialize_system();//allocate memory for all entities
  player_init(); //creates player entity
  tile_init_system();
  InitMouse2();
  atexit(CleanUpAll);
}

int getImagePathFromFile(char *filepath,char * filename)
{
    FILE *fileptr = NULL;
    char buf[255];
    int returnValue = -1;
    if (!filepath)
    {
        fprintf(stdout,"getImagePathFromFile: warning, no output parameter provided\n");
        return -1;
    }
    if (!filename)
    {
        fprintf(stdout,"getImagePathFromFile: warning, no input file path provided\n");
        return -1;
    }
    fileptr = fopen(filename,"r");
    if (!fileptr)
    {
        fprintf(stderr,"unable to open file: %s\n",filename);
        return -1;
    }
    if (fscanf(fileptr,"%s",buf))
    {
        if (strcmp(buf,"image:")==0)
        {
            fscanf(fileptr,"%s",filepath);
            returnValue = 0;
        }
    }
    fclose(fileptr);
    return returnValue;
}

void addCoordinateToFile(char *filepath,int x, int y)
{
    FILE *fileptr = NULL;
    if (!filepath)
    {
        fprintf(stdout,"addCoordinateToFile: warning, no input file path provided\n");
        return;
    }
    fileptr = fopen(filepath,"a");
    if (!fileptr)
    {
        fprintf(stderr,"unable to open file: %s\n",filepath);
        return;
    }
    fprintf(fileptr,"%s:%i:newcoordinate: %i %i\n",__FILE__,__LINE__,x,y);
    fclose(fileptr);
}

int getCoordinatesFromFile(int *x, int *y,char * filename)
{
    FILE *fileptr = NULL;
    char buf[255];
    int tx,ty;
    int returnValue = -1;
    if ((!x)&&(!y))
    {
        fprintf(stdout,"getCoordinatesFromFile: warning, no output parameter provided\n");
        return -1;
    }
    if (!filename)
    {
        fprintf(stdout,"getCoordinatesFromFile: warning, no input file path provided\n");
        return -1;
    }
    fileptr = fopen(filename,"r");
    if (!fileptr)
    {
        fprintf(stderr,"unable to open file: %s\n",filename);
        return -1;
    }
    while (fscanf(fileptr,"%s",buf) != EOF)
    {
        fprintf(stdout,"buf is: %s\n",buf);
        if (strcmp(buf,"position:")==0)
        {
            fscanf(fileptr,"%i %i",&tx,&ty);
            fprintf(stdout,"as read: %i, %i\n",tx,ty);
            returnValue = 0;
        }
    }
    fclose(fileptr);
    if (x)*x = tx;
    if (y)*y = ty;
    return returnValue;
}
