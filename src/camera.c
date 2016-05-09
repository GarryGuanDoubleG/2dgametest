#include "Camera.h"
#include "types.h"
static SDL_Rect G_Camera = {0,0,1280,720};
static Vec2d G_Offset;

void Camera_Init(Vec2d pos, int width, int height)
{
	G_Camera.x = pos.x;
	G_Camera.y = pos.y;
	G_Camera.w = width;
	G_Camera.h = height;
}

void Camera_Set_Editor_Offset(Vec2d offset)
{
	Vec2dCopy(offset, G_Offset);
}

Vec2d Camera_Get_Editor_Offset()
{
	Vec2d offset;

	Vec2dCopy(G_Offset, offset);
	return offset;
}

SDL_Rect Camera_Get_Camera()
{
	return G_Camera;
}
Vec2d Camera_GetPosition()
{
	Vec2d pos;
	Vec2dSet(pos, G_Camera.x, G_Camera.y);
	return pos;
}
Vec2d Camera_GetSize()
{
	Vec2d size;
	Vec2dSet(size, G_Camera.w, G_Camera.h);
	return size;
}
void Camera_SetPosition(Vec2d pos)
{
	G_Camera.x = pos.x;
	G_Camera.y = pos.y;
}
void Camera_SetSize(Vec2d size)
{
	G_Camera.w = size.x;
	G_Camera.h = size.y;
}
SDL_Rect Camera_Bounds()
{
	SDL_Rect bounds;
	bounds.x = G_Camera.x+130;
	bounds.y = G_Camera.y+150;
	bounds.w = 745;
	bounds.h = 400;

	return bounds;
}
