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

void physics_manager::add_physics(PhysicsComponent* ptr)
{
	//if (ptr == nullptr)
	//	return;
	m_components.emplace(ptr);
	m_engine->notify_ptr_added(ptr);
}

void physics_manager::remove_physics(PhysicsComponent* ptr)
{
	//if (ptr == nullptr)
	//	return;
	m_components.erase(ptr);
	m_engine->notify_ptr_removed(ptr);
}

physics_manager::~physics_manager()
{
	delete m_engine;
}
