#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "vector.h"
#include "sprite.h"

typedef struct Vector2D{
	float x,y;
	void (add*)();
	float (getLength*)();
	void set();
	void negate();
	void scale();
	void rotate();
	void normalize();
	void crossProduct();
}
typedef struct Vector3D{

}

Typedef struct Entity_S
{
	int inuse;
	Sprite2 * sprite,
	int frame,
	Vect2d position;
	Vect2d velocity;
	//audio library
	int health, maxhealth,
	int inventory[],
	int stamina,
	int state,
	int velocityX, velocityY,
	void (think*)(struct Entity_S *self);
	void free();
	void initialize_system();
	void load();
	void close();
	void draw(int x, int y);

}Entity;

#endif