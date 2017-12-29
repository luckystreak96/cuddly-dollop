#ifndef COLLISION_MANAGER_H__
#define COLLISION_MANAGER_H__

#include "physics_2d.h"

class CollisionManager
{
public:
	CollisionManager();
	CollisionManager(std::map<unsigned int, std::shared_ptr<Entity>>* e, std::vector<std::shared_ptr<MapTile>>* t);
	std::map<unsigned int, std::shared_ptr<Entity>> CalculateCollision();
	void SetEntities(std::map<unsigned int, std::shared_ptr<Entity>>* e);
	void SetMapTiles(std::vector<std::shared_ptr<MapTile>>* t);
private:
	std::map<unsigned int, std::shared_ptr<Entity>>* m_entities;
	std::vector<std::shared_ptr<MapTile>>* m_tiles;
};

#endif