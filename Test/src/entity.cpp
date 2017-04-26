#include "entity.h"

Vector3f Entity::Velocity()
{
	return m_velocity;
}

Vector3f Entity::Position()
{
	return m_pos;
}

void Entity::Move(float elapsedTime)
{
	m_pos += m_velocity * elapsedTime;
}

Vector3f Entity::Emulate(float elapsedTime)
{
	return m_velocity * elapsedTime;
}

std::vector<Ent_Props>& Entity::GetProps()
{
	return m_props;
}

void Entity::AddProp(Ent_Props prop)
{
	m_props.push_back(prop);
}

bool Entity::ContainsProp(Ent_Props prop)
{
	for (std::vector<Ent_Props>::iterator it = m_props.begin(); it != m_props.end(); it++)
		if (*it == prop)
			return true;

	return false;
}