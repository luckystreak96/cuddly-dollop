#include "playerPhysicsComponent.h"

void PlayerPhysicsComponent::Update()
{
	PhysicsComponent::Update();
	PhysicsComponent::apply_natural_deceleration();
}