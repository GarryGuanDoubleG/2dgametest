#include "sprite.h"

Sprite			*spriteList = NULL;
int				G_Sprite_Count = 0;
Sprite			*Sprite_Mouse;
static int		G_Sprite_Max = 0;

struct
{
	Uint32 state;
	Uint32 shown;

	Uint32 frame;

	Uint16  x, y;
}Mouse;

/**
* @brief intializes the sprite for the mouse
*/
void Init_Mouse()
{
  if(!SDL_ShowCursor(false)){
	  printf("Could not hide mouse");
  }

  Sprite_Mouse = Sprite_Load("images/mouse.png", 16,16, NULL,NULL);
  if(Sprite_Mouse == NULL)fprintf(stdout,"mouse didn't load: %s\n", SDL_GetError());

  Mouse.state = 0;
  Mouse.shown = 0;
  Mouse.frame = 0;
}
/**
* @brief retrieves the mouse x and y position
* @return x and y position in a 2d vector
*/
Vec2d Get_Mouse_Pos()
{
	Vec2d mouse_pos = {Mouse.x, Mouse.y};
	return mouse_pos;
}

/**
* @brief Draw the mouse onto the screen
*/
void Draw_Mouse()
{
  int mx,my;
  int frame, fpl;
  int animations;

  SDL_Rect cam;
  Vec2d draw_pos;

  frame = Mouse.frame;
  fpl = 16;
  animations = 3; // specific to mouse

  SDL_GetMouseState(&mx,&my);

  cam = Graphics_Get_Player_Cam();
  Vec2dSet(draw_pos, mx + cam.x, my + cam.y);

  if(Sprite_Mouse != NULL)
	  Sprite_Draw(Sprite_Mouse, Mouse.frame, Graphics_Get_Renderer(), draw_pos);
  else
	  printf("Sprite_Mouse did not load properly");

  // go to next frame
  frame++;
  if( frame >= (animations * fpl))
  {
	  frame = 0;
  }

  Mouse.frame = frame;
  Mouse.x = mx;
  Mouse.y = my;
}

/**
* @brief allocates memory for a maximum 
* @param int maximum number of sprites to have allocated during game
*/
void Sprite_Initialize_System(int max_sprites)
{
	int i;

	if( max_sprites <= 0 )
	{
		fprintf(stdout, "The maximum number of sprites cannot be 0");
		return;
	}

	G_Sprite_Max = max_sprites;
	G_Sprite_Count = 0;

	spriteList = (Sprite *)malloc(sizeof(Sprite) * G_Sprite_Max);
	memset(spriteList, 0, sizeof(Sprite) * G_Sprite_Max);

	for(i = 0;i < G_Sprite_Max;i++)
	{
		spriteList[i].image= NULL;
	}

	atexit(Sprite_Close_System);
}

/**
* @brief deallocates the memory set during initialization
*/

void Sprite_Close_System()
{
	int i;
	Sprite  **target = NULL;
	Sprite *ptr = NULL;
	for(i = 0; i < G_Sprite_Max; i++){
		if( spriteList[i].refCount > 0){
			//target = spriteList[i];
			ptr = &spriteList[i];
			if(ptr){
				target = &ptr;
				Sprite_Free(target);
			}
		}
	}	
}

/**
* @brief loads a sprite from a filepath and initializses its sizes
* @param filepath to find and load sprite, img width and height for size of clip of sprite to load
  frame width and height for size of sprite in game
  @return pointer to loaded sprite
*/

Sprite *Sprite_Load(char *filename, int img_width, int img_height, int frameW, int frameH)
{
	SDL_Texture * newTexture = NULL;
	SDL_Surface * loadedSurface = NULL;
	Sprite* new_sprite;
	int i;

	if(!spriteList){
		fprintf(stdout, "Sprite List is null");
	}
	if(!filename){
		fprintf(stdout,"Filename in Sprite is Null\n");
		return NULL;
	}

	for(i = 0; i < G_Sprite_Count; i++){
		if(!strcmp(spriteList[i].filename, filename)){
			fprintf(stdout,"Sprite has already been loaded");
			return NULL;
		}
	}
	loadedSurface = IMG_Load(filename);
	if(!loadedSurface){
		fprintf(stdout,"%s was not loaded in Sprite_Load\n",filename);
		return NULL;
	}
	//SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF ) );

	newTexture = SDL_CreateTextureFromSurface(Graphics_Get_Renderer(), loadedSurface);
	if(!newTexture){
		printf("Unable to create New Texture from Sprite Surface");
		return NULL;
	}
	//Loop through sprite list to find unallocated memory location
	for(i = 0; i < G_Sprite_Max; i++){
		if((spriteList + i)->image == NULL){
			spriteList[i].refCount = 1;
			G_Sprite_Count++;

			//Get image dimensions
			spriteList[i].imageW = img_width;
			spriteList[i].imageH = img_height;
			spriteList[i].frameW = frameW;
			spriteList[i].frameH = frameH;
			spriteList[i].fpl	 = 16;

			spriteList[i].image = newTexture;
			spriteList[i].filename = filename;

			if(G_Sprite_Count +1 >= G_Sprite_Max){
				printf("Uh oh, max sprites reached!");
				exit(1);
			}

			SDL_FreeSurface(loadedSurface);
			return &spriteList[i];
		}		
	}


}

Sprite *Sprite_Load_Text(TTF_Font *font, char *text, SDL_Color color)
{
	int i;
	Sprite *sprite = NULL;
	SDL_Surface* text_surface = NULL;
	SDL_Texture* text_texture = NULL;
	SDL_Renderer *renderer;

	if(!font)return NULL;
	if(!text)return NULL;

	if(!spriteList)return NULL;

	renderer = Graphics_Get_Renderer();

	for(i = 0; i < G_Sprite_Max; i++)
	{
		if(spriteList[i].refCount == 0)
		{
			if(sprite == NULL)
			{
				sprite = &spriteList[i];
			}
			continue;
		}
		if(spriteList[i].filename == text)
		{
			spriteList[i].refCount++;
			return &spriteList[i];
		}
	}
	if(G_Sprite_Count + 1 > G_Sprite_Max)
	{
		slog("Maximum Sprites have been reached.");
		exit(1);
	}

	text_surface = TTF_RenderText_Blended(font, text, color);
	if(!text_surface)
	{
		return NULL;
	}
	else
	{
		text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	}
	if(!text_texture)
	{
		return NULL;
	}

	sprite->image = text_texture;
	sprite->filename = text;
	sprite->imageW = text_surface->w;
	sprite->imageH = text_surface->h;
	sprite->frameW = text_surface->w;
	sprite->frameH = text_surface->h;

	sprite->refCount++;
	sprite->fpl = 16;
	SDL_FreeSurface(text_surface);

	return sprite;
}


/**
* @brief frees the image loaded in the sprite and deallocates the memory for it
*/
void Sprite_Free(Sprite ** sprite)
{
	Sprite * target = *sprite;
	if(!sprite) return;
	if(!*sprite) return;
	//if(!*sprite) return;
	
	target->refCount--;

	if(target->refCount == 0)
	{
		/*just to be anal retentive, check to see if the image is already freed*/
		if(target->image != NULL)
		{
			SDL_DestroyTexture(target->image);
		}

		memset(target, 0, sizeof(Sprite));
		target->image = NULL;
	}
	target = NULL;
	sprite = NULL;
}

/**
* @brief draws the sprite onto the screen
* @param Sprite pointer to draw, frame_horizontal frame # to play, frame-vertical - which frame to play, renderer of game, and x and y offset to draw
*/

void Sprite_Draw(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d draw_pos)
{
	//Set rendering space and render to screen
	SDL_Rect camera;
	SDL_Rect src;
	SDL_Rect dest;

	camera = Graphics_Get_Player_Cam();

	src.x = frame%sprite->fpl * sprite->imageW;
	src.y = frame/sprite->fpl * sprite->imageH;
	src.w = sprite->imageW;
	src.h = sprite->imageH;

	dest.x =  draw_pos.x;
	dest.y = draw_pos.y; 
	dest.w = sprite->frameW;
	dest.h = sprite->frameH;

	if(rect_collide(camera, dest))
	{
		dest.x -= camera.x;
		dest.y -= camera.y;
		SDL_RenderCopy(renderer,sprite->image, &src, &dest);
	}
}

void Sprite_Text_Draw(Sprite *text, Vec2d draw_pos, int alpha)
{
	SDL_Rect src, dest;
	Vec2d pos;
	Vec2d pos_rel;
	Vec2d cam_pos;

	SDL_Renderer *renderer;
	SDL_Rect cam;
	
	pos = draw_pos;

	if(!text)return;

	renderer = Graphics_Get_Renderer();
	cam = Graphics_Get_Player_Cam();
	cam_pos.x = cam.x;
	cam_pos.y = cam.y;

	Vec2dSubtract(draw_pos, cam_pos, pos_rel); 

	src.x = 0;
	src.y = 0;
	src.w = text->imageW;
	src.h = text->imageH;
	
	dest.x = pos_rel.x;
	dest.y = pos_rel.y;
	dest.w = text->frameW;
	dest.h = text->frameH;

	SDL_SetTextureAlphaMod(text->image, alpha);
	SDL_RenderCopy(renderer, text->image, &src, &dest);
}