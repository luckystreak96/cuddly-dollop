#include "menuComponent.h"

std::shared_ptr<MenuComponent> MenuComponent::Update(bool active)
{
	m_active = active;
	return NULL;
}