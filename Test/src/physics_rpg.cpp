#include "physics_rpg.h"

#include "physics.h"

#include <algorithm>


physics_rpg::physics_rpg() : m_zone_range(1)
{
}

std::vector<PhysicsComponent*>& physics_rpg::get_touching_tiles(PhysicsComponent* ptr)
{
	if (m_last_touching_querier_id == ptr->_id)
		return m_temp_query;

	m_temp_query.clear();
	m_num_chunks = 0;

	auto bb1 = ptr->GetMoveBoundingBox();

	int h, v;
	h = v = -1;
	for (int i = 0; i < 2; i++)
	{
		if (h == get_list_position(bb1[i]))
			continue;
		h = get_list_position(bb1[i]);
		v = -1;
		for (int j = 0; j < 2; j++)
		{
			if (v == get_list_position(bb1[j + 2]))
				continue;
			v = get_list_position(bb1[j + 2]);
			resize_comps_static_list(h, v);
			for (auto& ts : m_comps_static[h][v])
			{
				if (ts->_ethereal)
					continue;

				auto bb2 = ts->GetMoveBoundingBox();

				if (Physics::Intersect2D(bb1, bb2))
					m_temp_query.push_back(ts);
			}
			++m_num_chunks;
		}
	}

	return m_temp_query;
}

float physics_rpg::collision_percent(PhysicsComponent* p, bool x_axis)
{
	std::array<float, 6> bb = x_axis ? p->GetXMoveBoundingBox() : p->GetYMoveBoundingBox();
	float velocity_movement = x_axis ? p->get_velocity_movement().x : p->get_velocity_movement().y;
	int right_up = x_axis ? Right : Up;
	int left_down = x_axis ? Left : Down;

	int floor_counter = 0;
	for (auto& tile : get_touching_tiles(p))
	{
		if (is_obstacle(p, tile) && Physics::Intersect2D(bb, tile->GetMoveBoundingBox()))
		{
			floor_counter = 0;
			break;
		}

		if (is_legal_floor(p, tile) && Physics::Intersect2D(bb, tile->GetMoveBoundingBox()))
			++floor_counter;
	}

	if (floor_counter < p->get_tile_expanse(bb))
	{
		if (velocity_movement > 0)
		{
			float bbside = p->GetBoundingBox()[right_up];
			return abs((bbside - ((int)(bbside / 64.0f) + 1.0f) * 64.0f) / 64.0f) / abs(velocity_movement);
		}
		//return abs(p->GetBoundingBox()[right_up] - floorf(p->GetBoundingBox()[right_up] + 1)) / abs(velocity_movement);
		else
		{
			float bbside = p->GetBoundingBox()[left_down];
			return abs((bbside - (int)(bbside / 64.0f) * 64.0f) / 64.0f) / abs(velocity_movement);
		}
		//return abs(p->GetBoundingBox()[left_down] - floorf(p->GetBoundingBox()[left_down])) / abs(velocity_movement);

	}

	return 2.0f;
}

bool physics_rpg::XY_collision(PhysicsComponent* p)
{
	int floor_counter = 0;
	for (auto& tile : get_touching_tiles(p))
	{
		if (is_obstacle(p, tile))
			return true;

		if (is_legal_floor(p, tile))
			++floor_counter;
	}

	if (floor_counter < m_num_chunks)
		return true;

	return false;
}

bool physics_rpg::is_legal_floor(PhysicsComponent* actor, PhysicsComponent* tile)
{
	return (tile->PositionRef().z >= actor->PositionRef().z + 0.495f && tile->PositionRef().z <= actor->PositionRef().z + 1.505f);
}

bool physics_rpg::is_obstacle(PhysicsComponent* actor, PhysicsComponent* tile)
{
	if (!tile->walkOn)
		return true;
	else if (tile->PositionRef().z > actor->PositionRef().z - 0.995f && tile->PositionRef().z <= actor->PositionRef().z + 0.25f)
		return true;

	return false;
}


void physics_rpg::calculate_collision()
{
	for (auto& phys : m_comps_dynamic)
	{
		if (phys->Velocity() != 0)
		{
			if (XY_collision(phys))
			{
				float x_col = collision_percent(phys, true);
				float y_col = collision_percent(phys, false);
				//std::cout << x_col << ", " << y_col << std::endl;
				if (x_col != 2 && y_col != 2)
				{
					phys->XCollide();
					phys->YCollide();
				}
				else if (x_col < y_col)
					phys->XCollide();
				else
					phys->YCollide();

				m_last_touching_querier_id = -1;
				phys->SetMovedBB();

				phys->_collided_last_frame = -2;
			}
		}
	}

	for (int i = 0; i < m_comps_dynamic.size(); i++)
	{
		PhysicsComponent* p1 = m_comps_dynamic[i];
		for (int j = i + 1; j < m_comps_dynamic.size(); j++)
		{
			PhysicsComponent* p2 = m_comps_dynamic[j];

			// Are the objects inside each other right now? (nothing will go fast enough to skip this)
			// if no collision, just skip
			if (!Physics::Intersect2D(p1->GetMoveBoundingBox(), p2->GetMoveBoundingBox())
				|| !is_obstacle(p1, p2) && !is_obstacle(p2, p1))
				continue;

			if (!p1->_ethereal && !p2->_ethereal)
				handle_dynamic_collision(i, j);

			p1->_collided_last_frame = p2->_id;
			p2->_collided_last_frame = p1->_id;
		}
	}
}

void physics_rpg::handle_dynamic_collision(int i, int j)
{
	bool i_left_of_j = m_comps_dynamic[i]->GetBoundingBox()[Right] < m_comps_dynamic[j]->GetBoundingBox()[Left];
	bool i_right_of_j = m_comps_dynamic[i]->GetBoundingBox()[Left] > m_comps_dynamic[j]->GetBoundingBox()[Right];
	bool i_under_of_j = m_comps_dynamic[i]->GetBoundingBox()[Up] < m_comps_dynamic[j]->GetBoundingBox()[Down];
	bool i_over_of_j = m_comps_dynamic[i]->GetBoundingBox()[Down] > m_comps_dynamic[j]->GetBoundingBox()[Up];

	if ((i_left_of_j || i_right_of_j) && (i_under_of_j || i_over_of_j))
	{
		float xperc = 0;
		if (i_left_of_j)
			xperc = abs(m_comps_dynamic[i]->GetMoveBoundingBox()[Right] - m_comps_dynamic[j]->GetMoveBoundingBox()[Left]);
		else
			xperc = abs(m_comps_dynamic[i]->GetMoveBoundingBox()[Left] - m_comps_dynamic[j]->GetMoveBoundingBox()[Right]);

		float yperc = 0;
		if (i_under_of_j)
			yperc = abs(m_comps_dynamic[i]->GetMoveBoundingBox()[Up] - m_comps_dynamic[j]->GetMoveBoundingBox()[Down]);
		else
			yperc = abs(m_comps_dynamic[i]->GetMoveBoundingBox()[Down] - m_comps_dynamic[j]->GetMoveBoundingBox()[Up]);

		if (xperc < yperc)
		{
			// do nothing, the collision will proceed properly in the next 'if' statement
		}
		else
			i_left_of_j = i_right_of_j = false;
	}

	if (i_left_of_j || i_right_of_j)
	{
		if ((m_comps_dynamic[i]->Velocity().x > 0 && i_left_of_j) || (m_comps_dynamic[i]->Velocity().x < 0 && i_right_of_j))
			m_comps_dynamic[i]->XCollide();
		if ((m_comps_dynamic[j]->Velocity().x > 0 && i_right_of_j) || (m_comps_dynamic[j]->Velocity().x < 0 && i_left_of_j))
			m_comps_dynamic[j]->XCollide();
	}
	else if (i_under_of_j || i_over_of_j)
	{
		if ((m_comps_dynamic[i]->Velocity().y > 0 && i_under_of_j) || (m_comps_dynamic[i]->Velocity().y < 0 && i_over_of_j))
			m_comps_dynamic[i]->YCollide();
		if ((m_comps_dynamic[j]->Velocity().y > 0 && i_over_of_j) || (m_comps_dynamic[j]->Velocity().y < 0 && i_under_of_j))
			m_comps_dynamic[j]->YCollide();
	}
}


bool fequals(float first, float second, float epsilon)
{
	return abs(first - second) < epsilon;
}

void physics_rpg::calculate_position_adjustments()
{
	for (auto& phys : m_comps_dynamic)
		if (phys->Velocity() != 0)
		{
			float tile_height = 100;
			for (auto& tile : get_touching_tiles(phys))
			{
				// get highest legal block
				if (is_legal_floor(phys, tile) && tile->PositionRef().z < tile_height)
					tile_height = tile->PositionRef().z;
			}

			if (tile_height != 100 && !fequals(tile_height - 1.f, phys->PositionRef().z, 0.05f))
			{
				float backup = phys->PositionRef().z;
				phys->PositionRef().z = tile_height - 1.f;
				for (auto& tile : get_touching_tiles(phys))
				{
					if (is_obstacle(phys, tile))
					{
						phys->PositionRef().z = backup;
						phys->XCollide();
						phys->YCollide();
						phys->_collided_last_frame = -2;
					}
				}
			}
		}

	m_last_touching_querier_id = -2;
}


void physics_rpg::notify_ptr_added(PhysicsComponent* ptr)
{
	if (ptr->_unmoving)
	{
		resize_comps_static_list(ptr);
		m_comps_static[get_x_list_position(ptr)][get_y_list_position(ptr)].push_back(ptr);
	}
	else
		m_comps_dynamic.push_back(ptr);
}


void physics_rpg::notify_ptr_removed(PhysicsComponent* ptr)
{
	if (ptr->_unmoving)
	{
		auto x = std::find(m_comps_static[get_x_list_position(ptr)][get_y_list_position(ptr)].begin(), m_comps_static[get_x_list_position(ptr)][get_y_list_position(ptr)].end(), ptr);
		m_comps_static[get_x_list_position(ptr)][get_y_list_position(ptr)].erase(x);
	}
	else
	{
		auto x = std::find(m_comps_dynamic.begin(), m_comps_dynamic.end(), ptr);
		m_comps_dynamic.erase(x);
	}
}


int physics_rpg::get_x_list_position(PhysicsComponent* ptr)
{
	return get_list_position(ptr->PositionRef().x);
}


int physics_rpg::get_y_list_position(PhysicsComponent* ptr)
{
	return get_list_position(ptr->PositionRef().y);
}

int physics_rpg::get_list_position(float p)
{
	// the +1 is for padding on the bottom and the left sides of the map
	// so you cant walk off screen because itll be considered a chunk :)
	return (p / 64.0f) + 1.f / m_zone_range;
}


void physics_rpg::resize_comps_static_list(PhysicsComponent* ptr)
{
	resize_comps_static_list(get_x_list_position(ptr), get_y_list_position(ptr));
	//while (m_comps_static.size() <= get_x_list_position(ptr))
	//	m_comps_static.push_back(std::vector<std::vector<PhysicsComponent*>>());

	//while (m_comps_static[get_x_list_position(ptr)].size() <= get_y_list_position(ptr))
	//	m_comps_static[get_x_list_position(ptr)].push_back(std::vector<PhysicsComponent*>());
}

void physics_rpg::resize_comps_static_list(int x, int y)
{
	while (m_comps_static.size() <= x)
		m_comps_static.push_back(std::vector<std::vector<PhysicsComponent*>>());

	while (m_comps_static[x].size() <= y)
		m_comps_static[x].push_back(std::vector<PhysicsComponent*>());
}
