#ifndef PHYSICS_2D_H__
#define PHYSICS_2D_H__

#include <vector>
#include "physics.h"
//#include "drawable.h"
#include <algorithm>
#include "entity.h"
#include "map_handler.h"
#include <array>

//struct CollidedSurfaces
//{
//	//CollidedSurfaces(Drawable* b, Drawable* f, Triangle t) : collider(b), collidee(f), surface(t) {}
//	//Drawable* collider;
//	//Drawable* collidee;
//	Triangle surface;
//	bool operator==(CollidedSurfaces cs)
//	{
//		if (cs.collidee == collidee && cs.collider == collider)
//			return true;
//		return false;
//	}
//};

enum Axis { X, Y, Z };

namespace Physics_2D
{
	//void Collision(std::vector<Drawable*>* list);
	std::vector<std::shared_ptr<Entity>> Collision(std::map<unsigned int, std::shared_ptr<Entity>>* list, std::shared_ptr<MapHandler> mh);
	bool IsLegalHeight(float tilez, float otherz);
	std::vector<std::shared_ptr<PhysicsComponent>> FindDupes(std::vector<std::shared_ptr<PhysicsComponent>>* list, float targetHeight);
	int TileTouchCount(std::array<float, 6> bb);
	void FindMinMaxLegalZ(std::vector<std::shared_ptr<PhysicsComponent>>* touching, float& min, float& max, float targetHeight);
	int TouchCount(std::vector<std::shared_ptr<PhysicsComponent>>* touching, float targetHeight);
	void ApplyCollision(std::shared_ptr<PhysicsComponent> a, std::shared_ptr<PhysicsComponent> b);
	void ApplyCollision(std::shared_ptr<PhysicsComponent> a, std::shared_ptr<PhysicsComponent> b, Axis precalculatedAxis);
}

#endif