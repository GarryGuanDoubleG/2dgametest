#include "vector.h"
#include <math.h>
#include "simple_logger.h"


#define Vec2dAdd(a,b,c) ( c.x = a.x + b.x, c.y = a.y + b.y)
#define Vec3dAdd(a,b,c) ( c.x = a.x + b.x, c.y = a.y + b.y, c.z = a.z + b.z)
#define Vec4dAdd(a,b,c) ( c.x = a.x + b.x, c.y = a.y + b.y, c.z = a.z + b.z, c.w = a.w + b.w)

#define Vec2dSubtract(a,b,c) ( c.x = a.x - b.x, c.y = a.y - b.y)
#define Vec3dSubtract(a,b,c) ( c.x = a.x - b.x, c.y = a.y - b.y, c.z = a.z - a.y)
#define Vec4dSubtract(a,b,c) ( c.x = a.x - b.x, c.y = a.y - b.y, c.z = a.z - a.y, c.w = a.w - b.w)

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

#define Vec2dSet(v, x, y)	(v.x = x, v.y = y)
#define Vec3dSet(v, x, y, z)	(v.x = x, v.y = y, v.z = z)
#define Vec4dSet(v, x, y, z, w)	(v.x = x, v.y = y, v.z = z, v.w = w)


float GetLength2d(Vec2d v){
	float number = (v.x * v.x) + (v.y * v.y);
	number = sqrt((double)number);

	return number;
}

float GetLength3d(Vec3d v){
	return sqrt( (double)(v.x * v.x) + (double)(v.y * v.y) + (double)(v.z * v.z));
}

float Normalize2d(Vec2d &v){
	float length;
	int ilen;
	length = GetLength2d(v);
	ilen = (int)length;
	if(length == 0)
	{
		slog("length is 0");
		slog("vel is x%i y%i", v.x, v.y);
		return length;
	}
	
	v.x = v.x / ilen;
	v.y = v.y / ilen;

	return length;
}

int Normalize3d(Vec3d &v){
	int length, ilength;	
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

void VectorScale(Vec2d *in, Vec2d *out, float scale)
{
	out->x = in->x * scale;
	out->y = in->y * scale;
}

SDL_Rect New_SDL_Rect(int x, int y, int w, int h)
{
	SDL_Rect new_rect;
	new_rect.x = x;
	new_rect.y = y;
	new_rect.w = w;
	new_rect.h = h;

	return new_rect;
}