#ifndef PHYSICS_RPG_H__
#define PHYSICS_RPG_H__

#include "physics_type.h"

#include <vector>

class physics_rpg : public physics_type
{
public:
	physics_rpg();
	void calculate_collision();
	void calculate_position_adjustments();

	void notify_ptr_added(PhysicsComponent* ptr);
	void notify_ptr_removed(PhysicsComponent* ptr);

private:
	// List operations
	int get_list_position(float p);
	int get_x_list_position(PhysicsComponent* ptr);
	int get_y_list_position(PhysicsComponent* ptr);
	void resize_comps_static_list(PhysicsComponent* ptr);
	void resize_comps_static_list(int x, int y);

	// Collisions
	bool XY_collision(PhysicsComponent* p);
	float collision_percent(PhysicsComponent* p, bool x_axis);
	bool is_obstacle(PhysicsComponent* actor, PhysicsComponent* tile);
	bool is_legal_floor(PhysicsComponent* actor, PhysicsComponent* tile);
	void handle_dynamic_collision(int first_index, int second_index);

	std::vector<PhysicsComponent*>& get_touching_tiles(PhysicsComponent* ptr);

private:
	int m_zone_range;

	// query data
	int m_last_touching_querier_id;
	int m_num_chunks;
	std::vector<PhysicsComponent*> m_temp_query;


	// Lists are sorted by [x][y][tile_in_that_range]
	std::vector<std::vector<std::vector<PhysicsComponent*>>> m_comps_static;
	std::vector<PhysicsComponent*> m_comps_dynamic;
};

#endif