#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "graphics.h"

#define MaxSprites    255

const int SCREEN_HEIGHT = 1120;
const int SCREEN_WIDTH = 1280;

struct
{
	Uint32 state;
	Uint32 shown;
	Uint32 frame;
	Uint16  x, y;
}Mouse;

SDL_Surface *buffer; /*pointer to the background image buffer*/
SDL_Surface *videobuffer; /*pointer to the draw buffer*/
SDL_Rect Camera; /*x & y are the coordinates for the background map, w and h are of the screen*/
Sprite SpriteList[MaxSprites];
Sprite *Msprite;
int NumSprites;
Uint32 NOW;					/*the current time since program started*/

SDL_Window   *   __gt_graphics_main_window = NULL;
SDL_Renderer *   __gt_graphics_renderer = NULL;
SDL_Texture  *   __gt_graphics_texture = NULL;
SDL_Surface  *   __gt_graphics_surface = NULL;
SDL_Surface  *   __gt_graphics_temp_buffer = NULL;


/*some data on the video settings that can be useful for a lot of functions*/
static int __gt_bitdepth;
static Uint32 __gt_rmask;
static Uint32 __gt_gmask;
static Uint32 __gt_bmask;
static Uint32 __gt_amask;

void gt_graphics_close();

void Init_Graphics(
	char *windowName,
    int viewWidth,
    int viewHeight,
    int renderWidth,
    int renderHeight,
    float bgcolor[4],
    int fullscreen)
{
    Uint32 flags = 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Unable to initilaize SDL system: %s",SDL_GetError());
        return;
    }
    atexit(SDL_Quit);
    if (fullscreen)
    {
        if (renderWidth == 0)
        {
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
        else
        {
            flags |= SDL_WINDOW_FULLSCREEN;
        }
    }
    __gt_graphics_main_window = SDL_CreateWindow(windowName,
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             renderWidth, renderHeight,
                             flags);

    if (!__gt_graphics_main_window)
    {
        printf("failed to create main window: %s",SDL_GetError());
        gt_graphics_close();
        return;
    }
    
    __gt_graphics_renderer = SDL_CreateRenderer(__gt_graphics_main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!__gt_graphics_renderer)
    {
        printf("failed to create renderer: %s",SDL_GetError());
        gt_graphics_close();
        return;
    }
    
    SDL_SetRenderDrawColor(__gt_graphics_renderer, 0, 0, 0, 255);
    SDL_RenderClear(__gt_graphics_renderer);
    SDL_RenderPresent(__gt_graphics_renderer);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(__gt_graphics_renderer, renderWidth, renderHeight);

    __gt_graphics_texture = SDL_CreateTexture(
        __gt_graphics_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        renderWidth, renderHeight);
    if (!__gt_graphics_texture)
    {
        printf("failed to create screen texture: %s",SDL_GetError());
        gt_graphics_close();
        return;
    };
    
    SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ARGB8888,
                                    &__gt_bitdepth,
                                    &__gt_rmask,
                                    &__gt_gmask,
                                    &__gt_bmask,
                                    &__gt_amask);

    
    __gt_graphics_surface = SDL_CreateRGBSurface(0, renderWidth, renderHeight, __gt_bitdepth,
                                        __gt_rmask,
                                    __gt_gmask,
                                    __gt_bmask,
                                    __gt_amask);
    buffer = SDL_CreateRGBSurface(0, renderWidth, renderHeight, __gt_bitdepth,
                                                 __gt_rmask,
                                                 __gt_gmask,
                                                 __gt_bmask,
                                                 __gt_amask);    
    if (!__gt_graphics_surface)
    {
        printf("failed to create screen surface: %s",SDL_GetError());
        gt_graphics_close();
        return;
    }
        
    atexit(gt_graphics_close);
    printf("graphics initialized\n");
}

void gt_graphics_render_surface_to_screen(SDL_Surface *surface,SDL_Rect srcRect,int x,int y)
{
    SDL_Rect dstRect;
    SDL_Point point = {1,1};
    int w,h;
    if (!__gt_graphics_texture)
    {
        printf("gt_graphics_render_surface_to_screen: no texture available");
        return;
    }
    if (!surface)
    {
        printf("gt_graphics_render_surface_to_screen: no surface provided");
        return;
    }
    SDL_QueryTexture(__gt_graphics_texture,
                     NULL,
                     NULL,
                     &w,
                     &h);
    /*check if resize is needed*/
    if ((surface->w > w)||(surface->h > h))
    {
        SDL_DestroyTexture(__gt_graphics_texture);
        __gt_graphics_texture = SDL_CreateTexture(__gt_graphics_renderer,
                                                   __gt_graphics_surface->format->format,
                                                   SDL_TEXTUREACCESS_STREAMING, 
                                                   surface->w,
                                                   surface->h);
        if (!__gt_graphics_texture)
        {
            printf("gt_graphics_render_surface_to_screen: failed to allocate more space for the screen texture!");
            return;
        }
    }
    SDL_SetTextureBlendMode(__gt_graphics_texture,SDL_BLENDMODE_BLEND);        
    SDL_UpdateTexture(__gt_graphics_texture,
                      &srcRect,
                      surface->pixels,
                      surface->pitch);
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = srcRect.w;
    dstRect.h = srcRect.h;
    SDL_RenderCopy(__gt_graphics_renderer,
                     __gt_graphics_texture,
                     &srcRect,
                     &dstRect);
}


void ResetBuffer()
{
    
}

void NextFrame()
{
  Uint32 Then;
  SDL_RenderPresent(__gt_graphics_renderer);
  Then = NOW;									/*these next few lines  are used to show how long each frame takes to update.  */
  NOW = SDL_GetTicks();
/*  fprintf(stdout,"Ticks passed this frame: %i\n", NOW - Then);*/
  FrameDelay(33); /*this will make your frame rate about 30 frames per second.  If you want 60 fps then set it to about 15 or 16*/
}

void gt_graphics_close()
{
    if (__gt_graphics_texture)
    {
        SDL_DestroyTexture(__gt_graphics_texture);
    }
    if (__gt_graphics_renderer)
    {
        SDL_DestroyRenderer(__gt_graphics_renderer);
    }
    if (__gt_graphics_main_window)
    {
        SDL_DestroyWindow(__gt_graphics_main_window);
    }
    if (__gt_graphics_surface)
    {
        SDL_FreeSurface(__gt_graphics_surface);
    }
    if (__gt_graphics_temp_buffer)
    {
        SDL_FreeSurface(__gt_graphics_temp_buffer);
    }
    __gt_graphics_surface = NULL;
    __gt_graphics_main_window = NULL;
    __gt_graphics_renderer = NULL;
    __gt_graphics_texture = NULL;
    __gt_graphics_temp_buffer = NULL;
}
/*
  InitSpriteList is called when the program is first started.
  It just sets everything to zero and sets all pointers to NULL.
  It should never be called again.
*/

void InitSpriteList()
{
  int x;
  NumSprites = 0;
  memset(SpriteList,0,sizeof(Sprite) * MaxSprites);
  for(x = 0;x < MaxSprites;x++)SpriteList[x].image = NULL;
}


/*Create a sprite from a file, the most common use for it.*/

Sprite *LoadSprite(char *filename,int sizex, int sizey)
{
  int i;
  SDL_Surface *temp;
  /*first search to see if the requested sprite image is alreday loaded*/
  for(i = 0; i < NumSprites; i++)
  {
    if(strncmp(filename,SpriteList[i].filename,20)==0)
    {
      SpriteList[i].used++;
      return &SpriteList[i];
    }
  }
  /*makesure we have the room for a new sprite*/
  if(NumSprites + 1 >= MaxSprites)
  {
        fprintf(stderr, "Maximum Sprites Reached.\n");
        exit(1);
  }
  /*if its not already in memory, then load it.*/
  NumSprites++;
  for(i = 0;i <= NumSprites;i++)
  {
    if(!SpriteList[i].used)break;
  }
  temp = IMG_Load(filename);
  if(temp == NULL)
  {
    fprintf(stderr,"unable to load a vital sprite: %s\n",SDL_GetError());
    exit(0);
  }
  SpriteList[i].image = temp;
  /*sets a transparent color for blitting.*/
  SDL_SetColorKey(SpriteList[i].image, SDL_TRUE , SDL_MapRGB(SpriteList[i].image->format, 255,255,255));
   /*then copy the given information to the sprite*/
  strncpy(SpriteList[i].filename,filename,20);
      /*now sprites don't have to be 16 frames per line, but most will be.*/
  SpriteList[i].framesperline = 16;
  SpriteList[i].w = sizex;
  SpriteList[i].h = sizey;
  SpriteList[i].used++;
  return &SpriteList[i];
}

/*the palette swapping version of LoadSprite.  It will check the file loaded to see if there is any pure colors for swapping them.*/

Sprite *LoadSwappedSprite(char *filename,int sizex, int sizey, int c1, int c2, int c3)
{
  int i;
  SDL_Surface *temp;
  /*first search to see if the requested sprite image is alreday loaded*/
  for(i = 0; i < NumSprites; i++)
  {
    if((strncmp(filename,SpriteList[i].filename,20)==0)&&(SpriteList[i].used >= 1)&&(c1 == SpriteList[i].color1)&&(c2 == SpriteList[i].color2)&&(c3 == SpriteList[i].color3))
    {
      SpriteList[i].used++;
      return &SpriteList[i];
    }
  }
  /*makesure we have the room for a new sprite*/
  if(NumSprites + 1 >= MaxSprites)
  {
        fprintf(stderr, "Maximum Sprites Reached.\n");
        exit(1);
  }
  /*if its not already in memory, then load it.*/
  NumSprites++;
  for(i = 0;i <= NumSprites;i++)
  {
    if(!SpriteList[i].used)break;
  }
  temp = IMG_Load(filename);
  if(temp == NULL)
  {
        fprintf(stderr, "FAILED TO LOAD A VITAL Sprite.\n");
        exit(1);
  }
  SpriteList[i].image = temp;
  /*sets a transparent color for blitting.*/
  SDL_SetColorKey(SpriteList[i].image, SDL_TRUE , SDL_MapRGB(SpriteList[i].image->format, 255,255,255));
  //fprintf(stderr,"asked for colors: %d,%d,%d \n",c1,c2,c3);
  SwapSprite(SpriteList[i].image,c1,c2,c3);
   /*then copy the given information to the sprite*/
  strcpy(SpriteList[i].filename,filename);
      /*now sprites don't have to be 16 frames per line, but most will be.*/
  SpriteList[i].framesperline = 16;
  SpriteList[i].w = sizex;
  SpriteList[i].h = sizey;
  SpriteList[i].color1 = c1;
  SpriteList[i].color2 = c2;
  SpriteList[i].color3 = c3;
  SpriteList[i].used++;
  return &SpriteList[i];
}

/*
 * When we are done with a sprite, lets give the resources back to the system...
 * so we can get them again later.
 */

void FreeSprite(Sprite *sprite)
{
  /*first lets check to see if the sprite is still being used.*/
  sprite->used--;
  if(sprite->used == 0)
  {
  strcpy(sprite->filename,"\0");
     /*just to be anal retentive, check to see if the image is already freed*/
  if(sprite->image != NULL)SDL_FreeSurface(sprite->image);
  sprite->image = NULL;
  }
 /*and then lets make sure we don't leave any potential seg faults 
  lying around*/
}

void CloseSprites()
{
  int i;
   for(i = 0;i < MaxSprites;i++)
   {
     /*it shouldn't matter if the sprite is already freed, 
     FreeSprite checks for that*/
      FreeSprite(&SpriteList[i]);
   }
}

void DrawSprite(Sprite *sprite,SDL_Surface *surface,int sx,int sy, int frame)
{
    SDL_Rect src,dest;
    src.x = frame%sprite->framesperline * sprite->w;
    src.y = frame/sprite->framesperline * sprite->h;
    src.w = sprite->w;
    src.h = sprite->h;
    dest.x = sx;
    dest.y = sy;
    dest.w = sprite->w;
    dest.h = sprite->h;
    SDL_BlitSurface(sprite->image, &src, surface, &dest);
  
}

/*
	This will draw a pixel on the surface that is past at the x and y coordinates of the color given;
*/

void DrawPixel(SDL_Surface *screen, Uint8 R, Uint8 G, Uint8 B, int x, int y)
{
    Uint32 color = SDL_MapRGB(screen->format, R, G, B);

    if ( SDL_MUSTLOCK(screen) )
    {
        if ( SDL_LockSurface(screen) < 0 )
        {
            return;
        }
    }
    switch (screen->format->BytesPerPixel)
    {
        case 1:
        { /* Assuming 8-bpp */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *bufp = color;
        }
        break;

        case 2:
        { /* Probably 15-bpp or 16-bpp */
            Uint16 *bufp;

            bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
            *bufp = color;
        }
        break;

        case 3:
        { /* Slow 24-bpp mode, usually not used */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *(bufp+screen->format->Rshift/8) = R;
            *(bufp+screen->format->Gshift/8) = G;
            *(bufp+screen->format->Bshift/8) = B;
        }
        break;

        case 4:
        { /* Probably 32-bpp */
            Uint32 *bufp;

            bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
            *bufp = color;
        }
        break;
    }
    if ( SDL_MUSTLOCK(screen) )
    {
        SDL_UnlockSurface(screen);
    }
}


Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    /* Here p is the address to the pixel we want to retrieve*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;

    switch(surface->format->BytesPerPixel)
    {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /*shouldn't happen, but avoids warnings*/
    }
}



/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;

    switch(surface->format->BytesPerPixel)
    {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}


/*
  makes sure a minimum number of ticks is waited between frames
  this is to ensure that on faster machines the game won't move so fast that
  it will look terrible.
  This is a very handy function in game programming.
*/

void FrameDelay(Uint32 delay)
{
    static Uint32 pass = 100;
    Uint32 dif;
    dif = SDL_GetTicks() - pass;
    if(dif < delay)SDL_Delay( delay - dif);
    pass = SDL_GetTicks();
}

/*sets an sdl surface to all color.*/

void BlankScreen(SDL_Surface *buf,Uint32 color)
{
    SDL_LockSurface(buf);
    memset(buf->pixels, (Uint8)color,buf->format->BytesPerPixel * buf->w *buf->h);
    SDL_UnlockSurface(buf);
}
/*
 * This is the beginning of my Palette swapping scheme.  It checks the value
 * of the color it is given to see if the given color is PURE red, PURE green,
 * or PURE blue.  If it is, it takes the value as a percentage to apply to
 * the new color.  It returns either the old color untouched (if it wasn't a 
 * special case) or the new color.
 */

Uint32 SetColor(Uint32 color, int newcolor1,int newcolor2, int newcolor3)
{
    Uint8 r,g,b;
    Uint8 intensity;
    int newcolor;
    SDL_GetRGB(color, videobuffer->format, &r, &g, &b);
    if((r == 0) && (g == 0)&&(b !=0))
    {
        intensity = b;
        newcolor = newcolor3;
    }
    else if((r ==0)&&(b == 0)&&(g != 0))
    {
        intensity = g;
        newcolor = newcolor2;
    }
    else if((g == 0)&&(b == 0)&&(r != 0))
    {
        intensity = r;
        newcolor = newcolor1;
    }
    else return color;
    switch(newcolor)
    {
        case Red:
            r = intensity;
            g = 0;
            b = 0;
            break;
        case Green:
            r = 0;
            g = intensity;
            b = 0;
            break;
        case Blue:
            r = 0;
            g = 0;
            b = intensity;
            break;
        case Yellow:
            r = (Uint8)(intensity * 0.7);
            g = (Uint8)(intensity * 0.7);
            b = 0;
            break;
        case Orange:
            r = (Uint8)(intensity * 0.9);
            g = (Uint8)(intensity * 0.4);
            b = (Uint8)(intensity * 0.1);
            break;
        case Violet:
            r = (Uint8)(intensity * 0.7);
            g = 0;
            b = (Uint8)(intensity * 0.7);
            break;
        case Brown:
            r = (Uint8)(intensity * 0.6);
            g = (Uint8)(intensity * 0.3);
            b = (Uint8)(intensity * 0.15);
            break;
        case Grey:
            r = (Uint8)(intensity * 0.5);
            g = (Uint8)(intensity * 0.5);
            b = (Uint8)(intensity * 0.5);
            break;
        case DarkRed:
            r = (Uint8)(intensity * 0.5);
            g = 0;
            b = 0;
            break;
        case DarkGreen:
            r = 0;
            g = (Uint8)(intensity * 0.5);
            b = 0;
            break;
        case DarkBlue:
            r = 0;
            g = 0;
            b = (Uint8)(intensity * 0.5);
            break;
        case DarkYellow:
            r = (Uint8)(intensity * 0.4);
            g = (Uint8)(intensity * 0.4);
            b = 0;
            break;
        case DarkOrange:
            r = (Uint8)(intensity * 0.6);
            g = (Uint8)(intensity * 0.2);
            b = (Uint8)(intensity * 0.1);
            break;
        case DarkViolet:
            r = (Uint8)(intensity * 0.4);
            g = 0;
            b = (Uint8)(intensity * 0.4);
            break;
        case DarkBrown:
            r = (Uint8)(intensity * 0.2);
            g = (Uint8)(intensity * 0.1);
            b = (Uint8)(intensity * 0.05);
            break;
        case DarkGrey:
            r = (Uint8)(intensity * 0.3);
            g = (Uint8)(intensity * 0.3);
            b = (Uint8)(intensity * 0.3);
            break;
        case LightRed:
            r = intensity;
            g = (Uint8)(intensity * 0.45);
            b = (Uint8)(intensity * 0.45);
            break;
        case LightGreen:
            r = (Uint8)(intensity * 0.45);
            g = intensity;
            b = (Uint8)(intensity * 0.45);
            break;
        case LightBlue:
            r = (Uint8)(intensity * 0.45);
            b = intensity;
            g = (Uint8)(intensity * 0.45);
            break;
        case LightYellow:
            r = intensity;
            g = intensity;
            b = (Uint8)(intensity * 0.45);
            break;
        case LightOrange:
            r = intensity;
            g = (Uint8)(intensity * 0.75);
            b = (Uint8)(intensity * 0.35);
            break;
        case LightViolet:
            r = intensity;
            g = (Uint8)(intensity * 0.45);
            b = intensity;
            break;
        case LightBrown:
            r = intensity;
            g = (Uint8)(intensity * 0.85);
            b = (Uint8)(intensity * 0.45);
            break;
        case LightGrey:
            r = (Uint8)(intensity * 0.85);
            g = (Uint8)(intensity * 0.85);
            b = (Uint8)(intensity * 0.85);
            break;
        case Black:
            r = (Uint8)(intensity * 0.15);
            g = (Uint8)(intensity * 0.15);
            b = (Uint8)(intensity * 0.15);
            break;
        case White:
            r = intensity;
            g = intensity;
            b = intensity;
            break;
        case Tan:
            r = intensity;
            g = (Uint8)(intensity * 0.9);
            b = (Uint8)(intensity * 0.6);
            break;
        case Gold:
            r = (Uint8)(intensity * 0.8);
            g = (Uint8)(intensity * 0.7);
            b = (Uint8)(intensity * 0.2);
            break;
        case Silver:
            r = (Uint8)(intensity * 0.95);
            g = (Uint8)(intensity * 0.95);
            b = intensity;
            break;
        case YellowGreen:
            r = (Uint8)(intensity * 0.45);
            g = (Uint8)(intensity * 0.75);
            b = (Uint8)(intensity * 0.2);
            break;
        case Cyan:
            r = 0;
            g = (Uint8)(intensity * 0.85);
            b = (Uint8)(intensity * 0.85);
            break;
        case Magenta:
            r = (Uint8)(intensity * 0.7);
            g = 0;
            b = (Uint8)(intensity * 0.7);
            break;
    }
    return SDL_MapRGB(videobuffer->format,r,g,b);
}

/* This will probably never have to be called, returns the hex code for the
 * enumerated color
 */

Uint32 IndexColor(int color)
{
    switch(color)
    {
    case Red:
        return Red_;
    case Green:
        return Green_;
    case Blue:
        return Blue_;
    case Yellow:
        return Yellow_;
    case Orange:
        return Orange_;
    case Violet:
        return Violet_;
    case Brown:
        return Brown_;
    case Grey:
        return Grey_;
    case DarkRed:
        return DarkRed_;
    case DarkGreen:
        return DarkGreen_;
    case DarkBlue:
        return DarkBlue_;
    case DarkYellow:
        return DarkYellow_;
    case DarkOrange:
        return DarkOrange_;
    case DarkViolet:
        return DarkViolet_;
    case DarkBrown:
        return DarkBrown_;
    case DarkGrey:
        return DarkGrey_;
    case LightRed:
        return LightRed_;
    case LightGreen:
        return LightGreen_;
    case LightBlue:
        return LightBlue_;
    case LightYellow:
        return LightYellow_;
    case LightOrange:
        return LightOrange_;
    case LightViolet:
        return LightViolet_;
    case LightBrown:
        return LightBrown_;
    case LightGrey:
        return LightGrey_;
    case Black:
        return Black_;
    case White:
        return White_;
    case Tan:
        return Tan_;
    case Gold:
        return Gold_;
    case Silver:
        return Silver_;
    case YellowGreen:
        return YellowGreen_;
    case Cyan:
        return Cyan_;
    case Magenta:
        return Magenta_;
    }
    return Black_;
}
/*
 * and now bringing it all together, we swap the pure colors in the sprite out
 * and put the new colors in.  This maintains any of the artist's shading and
 * detail, but still lets us have that old school palette swapping.  
 */
void SwapSprite(SDL_Surface *sprite,int color1,int color2,int color3)
{
    int x, y;
    Uint32 pixel;
   /*First the precautions, that are tedious, but necessary*/
    if(color1 == -1)return;
    if(sprite == NULL)return;
    if ( SDL_LockSurface(sprite) < 0 )
    {
        fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
        exit(1);
    }
   /*now step through our sprite, pixel by pixel*/
    for(y = 0;y < sprite->h ;y++)
    {
        for(x = 0;x < sprite->w ;x++)
        {                           
             pixel = getpixel(sprite,x,y);/*and swap it*/
             putpixel(sprite,x,y,SetColor(pixel,color1,color2,color3));
        }
    }
    SDL_UnlockSurface(sprite);
}

/*mouse handling functions*/
/*this only handles the drawing and animation of.  Assuming you have a 16 by 16  tiled sprite sheet.  This will not handle input*/
void InitMouse()
{
  Msprite = LoadSprite("images/mouse.png",16, 16);
  if(Msprite == NULL)fprintf(stdout,"mouse didn't load: %s\n", SDL_GetError());
  Mouse.state = 0;
  Mouse.shown = 0;
  Mouse.frame = 0;
}

    /*draws to the screen immediately before the blit, after all
     it wouldn't be a very good mouse if it got covered up by the
     game content*/
void DrawMouse()
{
  int mx,my;
  SDL_GetMouseState(&mx,&my);
  if(Msprite != NULL) DrawSprite(Msprite,videobuffer,mx,my,Mouse.frame);
  Mouse.frame = (Mouse.frame + 1)%16;
 Mouse.x = mx;
 Mouse.y = my;
}

//GG edit

SDL_Window * get_gt_window(){
	return __gt_graphics_main_window;
}
SDL_Renderer * get_gt_renderer(){
	return __gt_graphics_renderer;
}
SDL_Texture * get_gt_texture(){
	return __gt_graphics_texture;
}
SDL_Surface * get_gt_Surface(){
	return __gt_graphics_surface;
}
SDL_Surface * get_gt_buffer(){
	return __gt_graphics_temp_buffer;
}
