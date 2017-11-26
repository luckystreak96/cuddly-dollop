#include "define.h"

std::map<Path, std::string> Paths::PathMap {
	std::make_pair(P_Tiles, "res/sprites/tiles/"),
	std::make_pair(P_Entities, "res/sprites/entities/"),
	std::make_pair(P_Particles, "res/sprites/particles/")
};

namespace Globals
{
	bool DEBUG_DRAW_NORMALS = false;
	bool DEBUG_DRAW_TILE_OUTLINES = false;
}