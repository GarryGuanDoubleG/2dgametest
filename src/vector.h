#ifndef _VECTOR_H_
#define _VECTOR_H_

/**
*@brief a simple 2d vector structure
*/

typedef struct{
	float x;/**< the x component of the vector */
	float y;/**< the y component of the vector */
}Vect2D;

typedef struct{
	union {float x; /**< the x component of the vector */
		   float r;}; /**< the red component of the color */
	union {float y;  /**< the y component of the vector */
		   float g;}; /**< the green component of the color */
	union {float z; /**< the z component of the vector */
		   float b;}; /**< the blue component of the color */
}Vect3d;

typedef struct{
	union {float x; 
		   float r;};
	union {float y;
		   float g;};
	union {float z; 
		   float b;};
	union {float w; 
		   float a;};
}Vect4d

#define vec2d.add(a,b,c) ( c.x = a.x + b.x, c.y = a.y + b.y)
#define vec3d.add(a,b,c) ( c.x = a.x + b.x, c.y = a.y + b.y, c.z = a.z + b.z)
#define vec3d.add(a,b,c) ( c.x = a.x + b.x, c.y = a.y + b.y)

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