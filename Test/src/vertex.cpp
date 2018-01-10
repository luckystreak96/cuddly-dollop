#include "vertex.h"

Vertex::Vertex() : vertex(Vector3f(0, 0, 0)), tex(Vector2f(0, 0)), color(Vector3f(1, 1, 1)), alpha(1) {}

Vertex::Vertex(Vector3f vec, Vector2f texx) : vertex(vec), tex(texx), color(Vector3f(1, 1, 1)), alpha(1)
{
}

Vertex::Vertex(Vertex* v) : vertex(Vector3f(0, 0, 0)), tex(Vector2f(0, 0)), color(Vector3f(1, 1, 1)), alpha(1)
{
	vertex.x = v->vertex.x;
	vertex.y = v->vertex.y;
	vertex.z = v->vertex.z;

	tex.x = v->tex.x;
	tex.y = v->tex.y;

	color.x = v->color.x;
	color.y = v->color.y;
	color.z = v->color.z;

	alpha = v->alpha;
}