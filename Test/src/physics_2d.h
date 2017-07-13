#ifndef PHYSICS_VERTEX_H__
#define PHYSICS_VERTEX_H__

#include <vector>
#include "physics.h"
#include "drawable.h"
#include <algorithm>

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

enum Axis { X, Y, Z };

namespace Physics_2D
{
	void Collision(std::vector<Drawable*>* list);
	bool IsLegalHeight(float tilez, float otherz);
	std::vector<Drawable*> FindDupes(std::vector<Drawable*>* list, float targetHeight);
	int TileTouchCount(Array2d<float> bb);
	void FindMinMaxLegalZ(std::vector<Drawable*>* touching, float& min, float& max, float targetHeight);
	int TouchCount(std::vector<Drawable*>* touching, float targetHeight);
	void ApplyCollision(Drawable* a, Drawable* b);
	void ApplyCollision(Drawable* a, Drawable* b, Axis precalculatedAxis);
}

#endif