#ifndef MAP_TILE_H__
#define MAP_TILE_H__

#include <iostream>
#include <string>
#include "texture.h"
#include "model.h"
#include "math.h"
#include "physicsComponent.h"

/*
	TODO:
		Make the editor have a radio button for tile editing, entity mode and tile attribute editing
			The only tile attribute for now is walkOn, it must display something visually in that mode and toggle when you click
		Go inside physics2D -> make tiles with walkOn==false calculate collision a little differently (gotta figure out how tho xd)
*/

class MapTile
{
public:
	MapTile(Vector3f pos = Vector3f(), std::string modelName = "TILE", std::string texPath = "res/pattern.png", Vector3f bbsize = Vector3f(1, 1, 0), Vector3f centerSize = Vector3f(0, 0, 0));
	std::shared_ptr<PhysicsComponent> Physics() { return m_phys; }
	PhysicsComponent* PhysicsRaw() { return m_phys.get(); }
	std::string GetTexture() { return m_texPath; }
	Transformation* GetTransformation() { return &m_trans; }
	static inline bool SortFunc(std::shared_ptr<MapTile> d, std::shared_ptr<MapTile> d2) { return d->Physics()->Position().z > d2->Physics()->Position().z; }
private:
	std::shared_ptr<PhysicsComponent> m_phys;
	Transformation m_trans;
	std::string m_modelName;
	std::string m_texPath;
};

#endif
