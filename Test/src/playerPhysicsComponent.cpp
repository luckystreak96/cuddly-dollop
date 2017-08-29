#include "playerPhysicsComponent.h"

void PlayerPhysicsComponent::Update()
{
	PhysicsComponent::Update();
	PhysicsComponent::ApplyGravity();
}