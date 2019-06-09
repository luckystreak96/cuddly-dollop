#include "catch.hpp"

#include "../src/vertex.h"
#include "../src/physics_rpg.h"
#include "../src/map_tile.h"
#include "../src/playerPhysicsComponent.h"

#include <vector>

//TEST_CASE("c++ random tests", "[main]")
//{
//	int h = 0;
//	int v = 5;
//	REQUIRE(((h = v) == h));
//}

TEST_CASE("physics_rpg collision testing", "[physics_rpg]")
{
	// Init
	MapTile t1 = MapTile(Vector3f(0, 0, 5));
	t1.PhysicsRaw()->_unmoving = true;
	MapTile t2 = MapTile(Vector3f(1 * 64, 0, 5));
	t2.PhysicsRaw()->_unmoving = true;
	MapTile t3 = MapTile(Vector3f(1 * 64, 0, 4));
	t3.PhysicsRaw()->_unmoving = true;

	PhysicsComponent phys = PhysicsComponent(Vector3f(0.09 * 64.f, 0.15 * 64.f, 4.f), "TILE", Vector3f(0.8f * 64.f, 0.4f * 64.f, 0), Vector3f(1, 0, 0));
	phys._id = 1;
	PhysicsComponent phys2 = PhysicsComponent(Vector3f(0.9 * 64.f, 0.15 * 64.f, 4.f), "TILE", Vector3f(0.8f * 64.f, 0.4f * 64.f, 0), Vector3f(1, 0, 0));
	phys2._id = 2;
	phys.ActionMove(false, false, false, true, 10.0f, 0.0f);
	phys.DesiredMove();

	physics_rpg manager = physics_rpg();
	manager.notify_ptr_added(t1.PhysicsRaw());
	manager.notify_ptr_added(t2.PhysicsRaw());
	manager.notify_ptr_added(&phys);

	SECTION("moving on flat surface doesnt collide")
	{
		phys._collided_last_frame = -1;
		manager.calculate_collision();
		manager.calculate_position_adjustments();

		REQUIRE(phys._collided_last_frame == -1);
	}


	SECTION("moving into a block at the same height causes collision") {
		manager.notify_ptr_added(t3.PhysicsRaw());

		phys._collided_last_frame = -1;
		manager.calculate_collision();
		manager.calculate_position_adjustments();
		manager.notify_ptr_removed(t3.PhysicsRaw());

		REQUIRE(phys._collided_last_frame == -2);
	}

	SECTION("moving into a person at close height causes collision") {
		manager.notify_ptr_added(&phys2);

		phys._collided_last_frame = -1;
		manager.calculate_collision();
		manager.calculate_position_adjustments();
		//manager.notify_ptr_removed(t3.PhysicsRaw());

		REQUIRE(phys._collided_last_frame == 2);
		REQUIRE(phys2._collided_last_frame == 1);
	}

	SECTION("tile expanse function") {
		REQUIRE(phys.get_tile_expanse() == 2);
	}
}

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
		REQUIRE((x.alpha == 33.0f));
	}
}