#ifndef MAP_HANDLER_H__
#define MAP_HANDLER_H__

#include <iostream>
#include <string>
#include <time.h>
#include <vector>

#include "texture.h"
#include "math.h"
//#include "mesh.h"
#include "instance_mesh.h"
#include "map_tile.h"
#include "graphics_component.h"
#include "jsonHandler.h"


class MapHandler
{
public:
	MapHandler(unsigned int id, std::shared_ptr<JsonHandler> jh);
	~MapHandler();
	MapHandler(const std::string& filePath);
	void OrderTiles();
	void FinalizeSetup();
	void SetRender();
	void Draw();
	void Update(bool forced = false);
	std::vector<std::shared_ptr<MapTile>>* Tiles();
	std::vector<std::vector<std::vector<MapTile*>>>* OrderedTiles();
	static int GetXChunk(int x) { return x / m_chunkSize; }
	static int GetYChunk(int y) { return y / m_chunkSize; }
	unsigned int Size();
	void SetupMesh();
	std::shared_ptr<GraphicsComponent> Graphics() { return m_mesh.get_graphics(); }
//	void AdjustSprite(std::string sprite, Transformation& t, int index, bool firstTime);

	// Returns the farthest reaches of the map in  x, y and z
	Vector3f GetMapSize();
	static bool TileSort(std::shared_ptr<MapTile> i, std::shared_ptr<MapTile> j);
	static bool TileSortX(std::shared_ptr<MapTile> i, std::shared_ptr<MapTile> j);

private:
	std::vector<std::shared_ptr<MapTile>> m_tiles;
	std::vector<std::vector<std::vector<MapTile*>>> m_OrderedTiles;
	instance_mesh m_mesh;
	//GraphComp_ptr m_graphics = NULL;
	std::shared_ptr<JsonHandler> m_jsonHandler;
	int m_MBO_instances;
	unsigned int m_id;
	static int m_chunkSize;
	std::string m_texture;
	// The width height and depth of the map in tiles
	Vector3f m_mapSize;
};

#endif
