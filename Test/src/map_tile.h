#ifndef MAP_TILE_H__
#define MAP_TILE_H__

#include <iostream>
#include <string>
#include "texture.h"
#include "model.h"
#include "math.h"
#include "physicsComponent.h"

class MapTile
{
public:
	MapTile(Vector3f pos = Vector3f(), std::string modelName = "TILE", std::string texPath = "res/pattern.png", Vector3f bbsize = Vector3f(1, 1, 0), Vector3f centerSize = Vector3f(0, 0, 0));
	std::shared_ptr<PhysicsComponent> Physics() { return m_phys; }
	std::string GetTexture() { return m_texPath; }
	Transformation* GetTransformation() { return &m_trans; }
	static inline bool SortFunc(MapTile* d, MapTile* d2) { return d->Physics()->Position().z < d2->Physics()->Position().z; }
private:
	std::shared_ptr<PhysicsComponent> m_phys;
	Transformation m_trans;
	std::string m_modelName;
	std::string m_texPath;
};

#endif
