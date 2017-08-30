#ifndef MAP_HANDLER_H__
#define MAP_HANDLER_H__

#include <iostream>
#include <string>
#include "texture.h"
#include "model.h"
#include "array3d.h"
#include "math.h"
#include "mesh.h"
#include "map_tile.h"
#include "graphicsComponent.h"
#include <vector>

class MapHandler
{
public:
	MapHandler();
	~MapHandler();
	MapHandler(const std::string& filePath);
	void Draw();
	void Update();
	std::vector<MapTile*>* Tiles();
	unsigned int Size();
	void SetupMesh();
	GraphicsComponent* Graphics() { return m_graphics; }

private:
	std::vector<MapTile*> m_tiles = std::vector<MapTile*>();
	Mesh m_mesh;
	GraphicsComponent* m_graphics = NULL;
	int m_MBO_instances;
	std::string m_texture;
};

#endif
