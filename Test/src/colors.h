#ifndef COLORS_H__
#define COLORS_H__

#include "vector3f.h"

struct colors
{
	static Vector3f black;
	static Vector3f green;
};

Vector3f colors::black = Vector3f();
Vector3f colors::green = Vector3f(0.2f, 0.9f, 0.3f);

#endif