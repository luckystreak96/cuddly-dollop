#ifndef MAP_HANDLER_H__
#define MAP_HANDLER_H__

#include <iostream>
#include <string>
#include <time.h>
#include "texture.h"
#include "model.h"
#include "array3d.h"
#include "math.h"
#include "mesh.h"
#include "map_tile.h"
#include "graphicsComponent.h"
#include <vector>
#include "renderer.h"
#include "jsonHandler.h"
#include "particleGenerator.h"


class MapHandler
{
public:
	MapHandler();
	MapHandler(unsigned int id, std::shared_ptr<JsonHandler> jh);
	~MapHandler();
	MapHandler(const std::string& filePath);
	void FinalizeSetup();
	void SetRender();
	void Draw();
	void Update(bool forced = false);
	std::vector<std::shared_ptr<MapTile>>* Tiles();
	unsigned int Size();
	void SetupMesh();
	GraphComp_ptr Graphics() { return m_graphics; }

	// Returns the farthest reaches of the map in  x, y and z
	Vector3f GetMapSize();
	static inline bool TileSort(std::shared_ptr<MapTile> i, std::shared_ptr<MapTile> j) { return (i->Physics()->Position() < j->Physics()->Position()); }

private:
	std::vector<std::shared_ptr<MapTile>> m_tiles;
	Mesh m_mesh;
	GraphComp_ptr m_graphics = NULL;
	std::shared_ptr<JsonHandler> m_jsonHandler;
	int m_MBO_instances;
	unsigned int m_id;
	std::string m_texture;
	// The width height and depth of the map in tiles
	Vector3f m_mapSize;
};

#endif
