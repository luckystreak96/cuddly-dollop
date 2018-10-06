#include "catch.hpp"

#include "../Test/src/vertex.h"
#include <vector>

TEST_CASE("vertex color and alpha can be set", "[Vertex]")
{
	Vector3f color = Vector3f(4.0f);

	std::vector<Vertex> verts;
	for (int i = 0; i < 5; i++)
		verts.push_back(Vertex());

	Vertex::SetColorAll(verts.begin(), verts.end(), color, 33.0f);

	for (auto& x : verts)
	{
		REQUIRE((x.color == color));
		REQUIRE((x.alpha == 4.0f));
	}
}