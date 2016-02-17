#include "sprite.h"

Sprite2			*spriteList = NULL;
int				sprite_count = 0;
Sprite2			*Sprite_Mouse;
int		SPRITE_MAX = 0;

struct
{
	Uint32 state;
	Uint32 shown;
<<<<<<< HEAD
	Uint32 frame_horizontal, frame_vertical;
=======
	Uint32 frame_horizontal;
	Uint32 frame_vertical;
>>>>>>> f938880194512dc5de165eac4f3db7c7e5ae0628
	Uint16  x, y;
}Mouse2;

void InitMouse2()
{
  if(!SDL_ShowCursor(false)){
	  printf("Could not hide mouse");
  }

  Sprite_Mouse = sprite_load("images/mouse.png", 16,16);
  if(Sprite_Mouse == NULL)fprintf(stdout,"mouse didn't load: %s\n", SDL_GetError());
  Mouse2.state = 0;
  Mouse2.shown = 0;
<<<<<<< HEAD
  Mouse2.frame_horizontal = Mouse2.frame_vertical =  0;
=======
  Mouse2.frame_horizontal = Mouse2.frame_vertical = 0;
>>>>>>> f938880194512dc5de165eac4f3db7c7e5ae0628
}

void DrawMouse2()
{
  int mx,my;
  SDL_GetMouseState(&mx,&my);
  if(Sprite_Mouse != NULL)
	  sprite_draw(Sprite_Mouse,Mouse2.frame_horizontal, Mouse2.frame_vertical,__gt_graphics_renderer,mx,my);
  else
	  printf("Sprite_Mouse did not load properly");

  Mouse2.frame_horizontal = (Mouse2.frame_horizontal + 1)%16;
<<<<<<< HEAD
  if(Mouse2.frame_horizontal == 0)
	  Mouse2.frame_vertical = (Mouse2.frame_vertical + 1)%2;
=======
  if(Mouse2.frame_horizontal == 0){
	  Mouse2.frame_vertical = (Mouse2.frame_vertical +1)%3;
  }
>>>>>>> f938880194512dc5de165eac4f3db7c7e5ae0628
  Mouse2.x = mx;
  Mouse2.y = my;
}

void sprite_initialize_system(int max_sprites)
{
	int i;
	if( max_sprites <= 0 )
	{
		fprintf(stdout, "The maximum number of sprites cannot be 0");
		return;
	}
	SPRITE_MAX = max_sprites;
	sprite_count = 0;
	spriteList = (Sprite2 *)malloc(sizeof(Sprite2) * SPRITE_MAX);
	memset(spriteList, 0, sizeof(Sprite2) * SPRITE_MAX);
	for(i = 0;i < SPRITE_MAX;i++)spriteList[i].image= NULL;


	atexit(sprite_close_system);
}

void sprite_close_system()
{
	int i;
	Sprite2 ** target = NULL;
	for(i = 0; i < SPRITE_MAX; i++){
		if( spriteList[i].refCount > 0){
			//target = spriteList[i];
			sprite_free(&spriteList[i]);
		}
	}	
}

Sprite2 *sprite_load(char *filename, int width, int height)
{
	SDL_Texture * newTexture = NULL;
	SDL_Surface * loadedSurface = NULL;
	Sprite2* new_sprite;
	int i;
	if(!spriteList){
		fprintf(stdout, "Sprite List is null");
	}
	if(!filename){
		fprintf(stdout,"Filename in Sprite is Null\n");
		return NULL;
	}

	for(i = 0; i < SPRITE_MAX; i++){
		if(strcmp(spriteList[i].filename, filename)== true){
			fprintf(stdout,"Sprite has already been loaded");
			return NULL;
		}
	}
	loadedSurface = IMG_Load(filename);
	if(!loadedSurface){
		fprintf(stdout,"Surface was not loaded in sprite_load\n");
		return NULL;
	}
	SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF ) );

	newTexture = SDL_CreateTextureFromSurface(__gt_graphics_renderer, loadedSurface);
	if(!newTexture){
		printf("Unable to create New Texture from Sprite Surface");
		return NULL;
	}
	//Loop through sprite list to find unallocated memory location
	for(i = 0; i < SPRITE_MAX; i++){
		if((spriteList + i)->image == NULL){
			spriteList[i].refCount = 1;
			//Get image dimensions
			spriteList[i].imageW = width;
			spriteList[i].imageH = height;
			//global count
			sprite_count++;
			spriteList[i].image = newTexture;
			strncpy(spriteList[i].filename,filename,20);
			spriteList[i].fpl = 16;//frames per line. May have to change later

			spriteList[i].frameW = spriteList[i].imageW;
			spriteList[i].frameH = spriteList[i].imageH;
			if(sprite_count +1 >= SPRITE_MAX){
				printf("Uh oh, max sprites reached!");
				exit(1);
			}
			SDL_FreeSurface(loadedSurface);
			return &spriteList[i];
		}		
	}

}

void sprite_free(Sprite2 * sprite)
{
	Sprite2 *target = sprite;
	if(!sprite) return;
	//if(!*sprite) return;
  
	target->refCount--;

	if(target->refCount == 0)
	{
		strcpy(target->filename,"\0");
			/*just to be anal retentive, check to see if the image is already freed*/
		if(target->image != NULL)
			SDL_DestroyTexture(target->image);
		memset(target, 0, sizeof(Sprite2));

		target->image = NULL;
	}
	target = NULL;
 /*and then lets make sure we don't leave any potential seg faults 
  lying around*/
}

void sprite_draw(Sprite2 *sprite, int frame_horizontal, int frame_vertical, SDL_Renderer *renderer, int drawX, int drawY)
{
	//Set rendering space and render to screen
<<<<<<< HEAD
	SDL_Rect src = { frame_horizontal % sprite->fpl * sprite->imageW, 
=======
	SDL_Rect src = { frame_horizontal * sprite->imageW, 
>>>>>>> f938880194512dc5de165eac4f3db7c7e5ae0628
					 frame_vertical * sprite->imageH, 
					 sprite->imageW, sprite->imageH};
	SDL_Rect dest = { drawX, drawY, sprite->imageW, sprite->imageH};

	SDL_RenderCopy(renderer,sprite->image, &src, &dest);
}

/*eol@eof*/
