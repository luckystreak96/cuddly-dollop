#include "entity2.h"

Vector3f Entity2::Velocity()
{
	return m_velocity;
}

Vector3f Entity2::Position()
{
	return m_pos;
}

void Entity2::Move(float elapsedTime)
{
	m_pos += m_velocity * elapsedTime;
}

Vector3f Entity2::Emulate(float elapsedTime)
{
	return m_velocity * elapsedTime;
}

std::vector<Ent_Props>& Entity2::GetProps()
{
	return m_props;
}

void Entity2::AddProp(Ent_Props prop)
{
	m_props.push_back(prop);
}

bool Entity2::ContainsProp(Ent_Props prop)
{
	for (std::vector<Ent_Props>::iterator it = m_props.begin(); it != m_props.end(); it++)
		if (*it == prop)
			return true;

	return false;
}