#include "collisionManager.h"

CollisionManager::CollisionManager() : m_entities(NULL), m_tiles(NULL) {}


CollisionManager::CollisionManager(std::map<unsigned int, std::shared_ptr<Entity>>* e, std::vector<std::shared_ptr<MapTile>>* t) :
	m_entities(e), m_tiles(t)
{

}

void CollisionManager::SetEntities(std::map<unsigned int, std::shared_ptr<Entity>>* e)
{
	m_entities = e;
}
void CollisionManager::SetMapTiles(std::vector<std::shared_ptr<MapTile>>* t)
{
	m_tiles = t;
}

std::vector<std::shared_ptr<Entity>> CollisionManager::CalculateCollision()
{
	assert(m_entities != NULL);
	assert(m_tiles != NULL);
	return Physics_2D::CollisionRemastered(m_entities, m_tiles);
}
