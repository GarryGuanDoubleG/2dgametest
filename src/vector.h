#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "SDL.h"
/**
*@brief a simple 2d vector structure
*/

typedef struct{
	float x;/**< the x component of the vector */
	float y;/**< the y component of the vector */
}Vec2d;

typedef struct{
	union {float x; /**< the x component of the vector */
		   float r;}; /**< the red component of the color */
	union {float y;  /**< the y component of the vector */
		   float g;}; /**< the green component of the color */
	union {float z; /**< the z component of the vector */
		   float b;}; /**< the blue component of the color */
}Vec3d;

typedef struct{
	union {float x; 
		   float r;};
	union {float y;
		   float g;};
	union {float z; 
		   float b;};
	union {float w; 
		   float a;};
}Vec4d;

float GetLength2d(Vec2d v);
float GetLength3d(Vec3d v);
int rect_collide(SDL_Rect a, SDL_Rect b);

/*
void (add*)();
float (getLength*)();
void set();
void negate();
void scale();
void rotate();
void normalize();
void crossProduct();*/

#endif