#include "catch.hpp"

#include "../src/vertex.h"
#include <vector>

TEST_CASE("vertex color and alpha can be set", "[Vertex]")
{
	std::vector<Vertex> verts;
	for(int i = 0; i < 5; i++)
		verts.push_back(Vertex());

	Vertex::SetColorAll(verts.begin(), verts.end(), Vector3f(4.0f))
}