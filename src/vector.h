#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "SDL.h"
/**
*@brief a simple 2d vector structure
*/
//SDL Rect using floats
typedef struct{
	float x;
	float y;
	float w;
	float h;
}Rect_f;
typedef struct{
	int x;/**< the x component of the vector */
	int y;/**< the y component of the vector */
}Vec2d;

typedef struct{
	union {int x; /**< the x component of the vector */
		   int r;}; /**< the red component of the color */
	union {int y;  /**< the y component of the vector */
		   int g;}; /**< the green component of the color */
	union {int z; /**< the z component of the vector */
		   int b;}; /**< the blue component of the color */
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
/*
int rect_collide(SDL_Rect a, SDL_Rect b);
<<<<<<< Updated upstream
=======
int rect_collide(Rect_f a, Rect_f b);*/


float Vec2dDistanceSQ(Vec2d a, Vec2d b);
float Vec2dDistance(Vec2d a, Vec2d b);

#define rect_collide(a,b) ( ((a.x + a.w > b.x ) && (b.x + b.w > a.x)&&(a.y + a.h > b.y ) && (b.y + b.h > a.y)) ? true : false)

#define Vec2dAdd(a,b,c) ( c.x = a.x + b.x, c.y = a.y + b.y)
#define Vec3dAdd(a,b,c) ( c.x = a.x + b.x, c.y = a.y + b.y, c.z = a.z + b.z)
#define Vec4dAdd(a,b,c) ( c.x = a.x + b.x, c.y = a.y + b.y, c.z = a.z + b.z, c.w = a.w + b.w)

#define Vec2dSub(a,b,c) ( c.x = a.x - b.x, c.y = a.y - b.y)
#define Vec3dSub(a,b,c) ( c.x = a.x - b.x, c.y = a.y - b.y, c.z = a.z - a.y)
#define Vec4dSub(a,b,c) ( c.x = a.x - b.x, c.y = a.y - b.y, c.z = a.z - a.y, c.w = a.w - b.w)

#define Vec2dCopy(a,b)			(b.x = a.x, b.y = b.x)
#define Vec3dCopy(a,b)		(b.x = a.x, b.y = b.x, b.z = a.z)
#define Vec4dCopy(a,b)		(b.x = a.x, b.y = b.x, b.z = a.z, , b.w = a.w)

#define Vec2dClear(a)			(a.x = a.y = 0)
#define Vec3dClear(a)			(a.x = a.y = a.z = 0)
#define Vec4dClear(a)			(a.x = a.y = a.z = a.w = 0)

#define DotProduct2d(a,b)			(a.x*b.x+a.y*b.y)
#define DotProduct3d(a,b)			(a.x*b.x+a.y*b.y + a.z * b.z)
#define DotProduct4d(a,b)			(a.x*b.x+a.y*b.y + a.z * b.z + a.w + b.w)

#define VectorNegate2d(a,b)		(b.x = -a.x, b.y = -a.y)
#define VectorNegate3d(a,b)		(b.x = -a.x, b.y = -a.y, b.z = -a.z)
#define VectorNegate4d(a,b)		(b.x = -a.x, b.y = -a.y, b.z = -a.z, b.w = -a.w)

#define VectorSet2d(v, x, y)	(v.x = x, v.y = y)
#define VectorSet3d(v, x, y, z)	(v.x = x, v.y = y, v.z = z)
#define VectorSet4d(v, x, y, z, w)	(v.x = x, v.y = y, v.z = z, v.w = w)

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