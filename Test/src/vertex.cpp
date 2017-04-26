#include "vertex.h"

Vertex::Vertex() : vertex(Vector3f(0, 0, 0)), tex(Vector2f(0, 0)), normal(Vector3f(0, 0, 0)), SpecularIntensity(5), SpecularPower(16) {}

Vertex::Vertex(Vector3f vec, Vector2f texx) : vertex(vec), tex(texx), normal(Vector3f(0, 0, 0)), SpecularIntensity(5), SpecularPower(16)
{
}

Vertex::Vertex(Vertex* v) : vertex(Vector3f(0, 0, 0)), tex(Vector2f(0, 0)), normal(Vector3f(0, 0, 0)), SpecularIntensity(5), SpecularPower(16)
{
	vertex.x = v->vertex.x;
	vertex.y = v->vertex.y;
	vertex.z = v->vertex.z;

	tex.x = v->tex.x;
	tex.y = v->tex.y;

	normal.x = v->normal.x;
	normal.y = v->normal.y;
	normal.z = v->normal.z;

	SpecularIntensity = v->SpecularIntensity;
	SpecularPower = v->SpecularPower;
}