#ifndef _LTexture_H
#define _LTexture_H

#include "graphics.h"
#include "SDL_ttf.h"
#include <stdio.h>
#include <string>
#include <fstream>


//Texture wrapper class obtained from lazyfoo.net
//copyrighted by lazyfoo

//The window we'll be rendering to
extern SDL_Window* gWindow;
//The window renderer
extern SDL_Renderer* gRenderer;
extern TTF_Font *gFont;

//Scene textures
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

bool init();
//Frees media and shuts down SDL
void close();


#endif