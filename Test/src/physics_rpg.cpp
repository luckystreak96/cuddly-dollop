#include "physics_rpg.h"

physics_rpg::physics_rpg() : m_initialized(false)
{
	int i = 0;
}

void physics_rpg::calculate_collision()
{
	// at this point in the code its safe to assume all PhysicsComponents are created
	if (!m_initialized)
	{
		// initialize optimizations on lists here
		m_initialized = true;
	}
}

void physics_rpg::calculate_position_adjustments()
{

}