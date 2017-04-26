#ifndef PHYSICS_VERTEX_H__
#define PHYSICS_VERTEX_H__

#include <vector>
#include "physics.h"
#include "drawable.h"

struct CollidedSurfaces
{
	CollidedSurfaces(Drawable* b, Drawable* f, Triangle t) : collider(b), collidee(f), surface(t) {}
	Drawable* collider;
	Drawable* collidee;
	Triangle surface;
	bool operator==(CollidedSurfaces cs)
	{
		if (cs.collidee == collidee && cs.collider == collider)
			return true;
		return false;
	}
};

namespace Physics_Vertex
{
	void VertexCollision(std::vector<Drawable*>* list);
}

#endif