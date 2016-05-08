#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "sprite.h"
#include "simple_logger.h"
#include "vector.h"
#include "Menu.h"

TTF_Font *font = NULL;
TTF_Font *font2 = NULL;

SDL_Color Text_Color = { 200, 200, 200 };

Sprite *bg;
Vec2d G_bg_pos;

void Menu_Title_Screen_Draw()
{
	SDL_Renderer* renderer = Graphics_Get_Renderer();

	Text * text_start;
	
	Vec2d start_text_pos;

	int random;

	const Uint8 *keys;
	SDL_Event e;

	int done;

	font = TTF_OpenFont("fonts/menu.ttf", 36);
	font2 = TTF_OpenFont("fonts/menu.ttf", 18);
	bg = Sprite_Load("images/Title Screen/menu.png",1280,720, 1280, 720);

	if(!bg) return;
	if(font == NULL)
	{
		slog("Error loading font: %s", TTF_GetError());
		return;
	}
	if(font2 == NULL)return;

	done = 0;

	text_start = Sprite_Load_Text(font, "Press Enter to Start", Text_Color);

	Vec2dSet(G_bg_pos, 0, 0);
	Vec2dSet(start_text_pos, 640, 360);

	Sprite_Draw(bg, 0,renderer,G_bg_pos.x, G_bg_pos.y);

	do 
	{	
		random = rand() % 150+100;	
		Sprite_Text_Draw(text_start ,start_text_pos, random);
		
		SDL_RenderPresent(renderer);
		SDL_PollEvent (&e);
		if(&e)
		{
			if( e.type == SDL_KEYUP)
			{
				//Select Surface based on key presses
				switch(e.key.keysym.sym)
				{
				case SDLK_UP:
				case SDLK_w:
					e.type = SDLK_CLEAR;
					break;

				case SDLK_DOWN:
				case SDLK_s:
					e.type = SDLK_CLEAR;
					break;
				case SDLK_RETURN:
				case SDLK_SPACE:
					done = 1; 
					e.type = SDLK_CLEAR;
					break;

				}
			}
		}
	keys = SDL_GetKeyboardState(NULL);
	if(keys[SDL_SCANCODE_ESCAPE])
		{
			exit(1);
		}

	}
	while(!done);
	return;
}

void Menu_Main_Draw()
{
	SDL_Renderer* renderer = Graphics_Get_Renderer();

	const Uint8 *keys;
	SDL_Event e;

	int done;
	int select = 0;

	Text *NewGame;
	Text *LoadGame;
	Text *QuitGame;

	Vec2d NGPos;
	Vec2d LGPos;
	Vec2d QuitPos;

	Vec3d color;

	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	if(font == NULL)return;

	NewGame = Sprite_Load_Text(font,"New Game", Text_Color);
	LoadGame = Sprite_Load_Text(font,"Load Game", Text_Color);
	QuitGame = Sprite_Load_Text(font,"Quit Game", Text_Color);

	Vec2dSet(NGPos, 480, 360);
	Vec2dSet(LGPos, 480, 410);
	Vec2dSet(QuitPos, 480, 460);

	Vec3dSet(color, 254,254,254);

	Sprite_Draw(bg,0,renderer, G_bg_pos.x, G_bg_pos.y);

	Sprite_Text_Draw(NewGame, NGPos, 255);
	
	Sprite_Text_Draw(LoadGame, LGPos, 255);

	//sprite_BloomDraw(QuitGame,0,renderer,QuitPos, color, 5);
	Sprite_Text_Draw(QuitGame, QuitPos, 255);

	done = 0;

	do
	{
		Sprite_Text_Draw(NewGame, NGPos, 255);
		Sprite_Text_Draw(LoadGame, LGPos, 255);
		Sprite_Text_Draw(QuitGame, QuitPos, 255);

		SDL_RenderPresent(renderer);
		switch(select)
		{
			case 0:
				//sprite_BloomDraw(NewGame,0,renderer,NGPos, color, 5);
				
				//Sprite_Text_Draw(LoadGame, LGPos);
				//Sprite_Text_Draw(QuitGame, QuitPos);
				SDL_SetTextureColorMod(NewGame->image,255,255,255);

				SDL_SetTextureColorMod(LoadGame->image, 28,1,13);
				SDL_SetTextureColorMod(QuitGame->image, 28,1,13);
			break;

			case 1:
				//sprite_BloomDraw(LoadGame,0,renderer,LGPos, color, 5);

				//Sprite_Text_Draw(NewGame, NGPos);
				//Sprite_Text_Draw(QuitGame, QuitPos);

				SDL_SetTextureColorMod(LoadGame->image,255,255,255);

				SDL_SetTextureColorMod(NewGame->image, 28,1,13);
				SDL_SetTextureColorMod(QuitGame->image, 28,1,13);
				break;
		
			case 2:
				//sprite_BloomDraw(QuitGame,0,renderer,QuitPos, color, 5);

				//Sprite_Text_Draw(NewGame, NGPos);
				//Sprite_Text_Draw(LoadGame, LGPos);
				SDL_SetTextureColorMod(QuitGame->image,255,255,255);

				SDL_SetTextureColorMod(NewGame->image, 28,1,13);
				SDL_SetTextureColorMod(LoadGame->image, 28,1,13);
				break;
		}

		SDL_PollEvent (&e);
		
		if(&e)
		{
			if(e.type == SDL_KEYUP)
			{
				switch(e.key.keysym.sym)
				{
					case SDLK_UP:
					case SDLK_w:
						if(select > 0)
						{
							select--;
						}
						e.type = SDLK_CLEAR;
						break;

					case SDLK_DOWN:
					case SDLK_s:
						if(select < 2)
						{
							select++;
						}
						e.type = SDLK_CLEAR;
						break;

					case SDLK_RETURN:
					case SDLK_SPACE:
						switch(select)
						{
						case 0:
							//Level_Load(0, 0);
							done = 1;
							break;
						case 1:		
							//Level_Load(0, 1);
							done = 1;
							break;
						case 2:
							exit(1);
							break;
						}
						break;
					}
			}
		}
		keys = SDL_GetKeyboardState(NULL);
		
		if(keys[SDL_SCANCODE_ESCAPE])
		{
			exit(1);
		}
	}
	while(!done);		

	return;
}