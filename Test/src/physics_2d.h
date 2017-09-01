#ifndef PHYSICS_VERTEX_H__
#define PHYSICS_VERTEX_H__

#include <vector>
#include "physics.h"
#include "drawable.h"
#include <algorithm>
#include "entity.h"
#include "map_handler.h"
#include <array>

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
	void Collision(std::vector<Entity*>* list);
	void Collision(std::vector<Entity*>* list, MapHandler* mh);
	bool IsLegalHeight(float tilez, float otherz);
	std::vector<Drawable*> FindDupes(std::vector<Drawable*>* list, float targetHeight);
	std::vector<Entity*> FindDupes(std::vector<Entity*>* list, float targetHeight);
	std::vector<PhysicsComponent*> FindDupes(std::vector<PhysicsComponent*>* list, float targetHeight);
	int TileTouchCount(std::array<float, 6> bb);
	void FindMinMaxLegalZ(std::vector<Drawable*>* touching, float& min, float& max, float targetHeight);
	void FindMinMaxLegalZ(std::vector<Entity*>* touching, float& min, float& max, float targetHeight);
	void FindMinMaxLegalZ(std::vector<PhysicsComponent*>* touching, float& min, float& max, float targetHeight);
	int TouchCount(std::vector<Drawable*>* touching, float targetHeight);
	int TouchCount(std::vector<Entity*>* touching, float targetHeight);
	int TouchCount(std::vector<PhysicsComponent*>* touching, float targetHeight);
	void ApplyCollision(Drawable* a, Drawable* b);
	void ApplyCollision(Entity* a, Entity* b);
	void ApplyCollision(PhysicsComponent* a, PhysicsComponent* b);
	void ApplyCollision(Drawable* a, Drawable* b, Axis precalculatedAxis);
	void ApplyCollision(Entity* a, Entity* b, Axis precalculatedAxis);
	void ApplyCollision(PhysicsComponent* a, PhysicsComponent* b, Axis precalculatedAxis);
}

#endif