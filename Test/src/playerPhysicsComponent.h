#ifndef PLAYER_PHYSICS_COMPONENT
#define PLAYER_PHYSICS_COMPONENT

#include "physicsComponent.h"

class PlayerPhysicsComponent : public PhysicsComponent
{
public:
	using PhysicsComponent::PhysicsComponent;
	void Update();
};

#endif // !PLAYER_PHYSICS_COMPONENT
