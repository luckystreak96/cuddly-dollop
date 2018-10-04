#include "physics_manager.h"

physics_manager::physics_manager(physics_type* engine) : m_engine(engine)
{
	m_engine->set_objects(&m_components);
}

void physics_manager::calculate_frame()
{
	m_engine->calculate_collision();
	m_engine->calculate_position_adjustments();
}


physics_manager::~physics_manager()
{
	delete m_engine;
}
