#include "vector.h"
#include <math.h>

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
int rect_collide(SDL_Rect a, SDL_Rect b){
	if((a.x + a.w > b.x ) && (b.x + b.w > a.x)&&
		(a.y + a.h > b.y ) && (b.y + b.h > a.y)){
			return true;
	}
	return false;
}

int rect_collide(Rect_f a, Rect_f b){
	if((a.x + a.w > b.x ) && (b.x + b.w > a.x)&&
		(a.y + a.h > b.y ) && (b.y + b.h > a.y)){
			return true;
	}
	return false;
}

int rect_collide(SDL_Rect a, Rect_f b){
	if((a.x + a.w > b.x ) && (b.x + b.w > a.x)&&
		(a.y + a.h > b.y ) && (b.y + b.h > a.y)){
			return true;
	}
	return false;
}*/

float GetLength2d(Vec2d v){
	return sqrt( (double)(v.x * v.x) + (double)(v.y * v.y));
}

float GetLength3d(Vec3d v){
	return sqrt( (double)(v.x * v.x) + (double)(v.y * v.y) + (double)(v.z * v.z));
}

float Normalize2d(Vec2d &v){
	float length, ilength;	
	length = GetLength2d(v);
	ilength = 1/length;
	
	v.x = v.x / ilength;
	v.y = v.y / ilength;

	return length;
}

float Normalize3d(Vec3d &v){
	float length, ilength;	
	length = GetLength3d(v);
	ilength = 1/length;
	
	v.x = v.x / ilength;
	v.y = v.y / ilength;
	v.y = v.z / ilength;

	return length;
}

float Vec2dDistanceSQ(Vec2d a, Vec2d b){
	return (b.x - a.x) *(b.x -a.x) + (b.y - a.y) * (b.y - a.y);
}

float Vec2dDistance(Vec2d a, Vec2d b){
	return sqrt((double)((b.x - a.x) *(b.x -a.x)) + (double)((b.y - a.y) * (b.y - a.y)));
}