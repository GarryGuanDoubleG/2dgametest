#include "vector.h"
#include <math.h>

int rect_collide(SDL_Rect a, SDL_Rect b){
	if((a.x + a.w > b.x ) && (b.x + b.w > a.x)&&
		(a.y + a.h > b.y ) && (b.y + b.h > a.y)){
			return true;
	}

	return false;
}

float GetLength2d(Vec2d v){
	return sqrt( (v.x * v.x + v.y * v.y));
}

float GetLength3d(Vec3d v){
	return sqrt( (v.x * v.x + v.y * v.y + v.z * v.z));
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
	return sqrt((b.x - a.x) *(b.x -a.x) + (b.y - a.y) * (b.y - a.y));
}