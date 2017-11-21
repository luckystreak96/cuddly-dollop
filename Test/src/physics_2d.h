#ifndef PHYSICS_2D_H__
#define PHYSICS_2D_H__

#include <vector>
#include "physics.h"
//#include "drawable.h"
#include <algorithm>
#include "entity.h"
#include "map_handler.h"
#include <array>
#include <linq\linq.h>
#include <boost\assign.hpp>

using boost::assign::list_of;
using boost::assign::map_list_of;

enum Axis { X, Y, Z };

namespace Physics_2D
{
	//void Collision(std::vector<Drawable*>* list);
	std::vector<std::shared_ptr<Entity>> Collision(std::map<unsigned int, std::shared_ptr<Entity>>* list, std::shared_ptr<MapHandler> mh);
	std::vector<std::shared_ptr<Entity>> CollisionRemastered(std::map<unsigned int, std::shared_ptr<Entity>>* list, std::vector<std::shared_ptr<MapTile>>* mt);
	bool IsLegalHeight(float tilez, float otherz);
	std::vector<std::shared_ptr<PhysicsComponent>> FindDupes(std::vector<std::shared_ptr<PhysicsComponent>>* list, float targetHeight);
	int TileTouchCount(std::array<float, 6> bb);
	void FindMinMaxLegalZ(std::vector<std::shared_ptr<PhysicsComponent>>* touching, float& min, float& max, float targetHeight);
	int TouchCount(std::vector<std::shared_ptr<PhysicsComponent>>* touching, float targetHeight);
	bool IsTileCollideHeight(float entityHeight, float colliderHeight);
	void TileCollision(std::shared_ptr<Entity> e, std::vector<std::shared_ptr<PhysicsComponent>> p);
	void ApplyCollision(std::shared_ptr<PhysicsComponent> a, std::shared_ptr<PhysicsComponent> b);
	void ApplyCollision(std::shared_ptr<PhysicsComponent> a, std::shared_ptr<PhysicsComponent> b, Axis precalculatedAxis);
}

#endif