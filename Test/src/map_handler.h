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

class MapHandler
{
public:
	MapHandler();
	~MapHandler();
	MapHandler(const std::string& filePath);
	void SetRender();
	void Draw();
	void Update();
	std::vector<MapTile*>* Tiles();
	unsigned int Size();
	void SetupMesh();
	GraphicsComponent* Graphics() { return m_graphics; }

	// Returns the farthest reaches of the map in  x, y and z
	Vector3f GetMapSize();
	static inline bool TileSort(MapTile* i, MapTile* j) { return (i->Physics()->Position() < j->Physics()->Position()); }

private:
	std::vector<MapTile*> m_tiles = std::vector<MapTile*>();
	Mesh m_mesh;
	GraphicsComponent* m_graphics = NULL;
	int m_MBO_instances;
	std::string m_texture;
	// The width height and depth of the map in tiles
	Vector3f m_mapSize;
};

#endif
