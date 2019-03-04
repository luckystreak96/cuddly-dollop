#ifndef PLAYER_PHYSICS_COMPONENT
#define PLAYER_PHYSICS_COMPONENT

#include "physicsComponent.h"
#include "input_manager.h"

class MovementState;

class PlayerPhysicsComponent : public PhysicsComponent
{
public:
	using PhysicsComponent::PhysicsComponent;
	PlayerPhysicsComponent(Vector3f pos, std::string modelName = "TILE", Vector3f size = Vector3f(), Vector3f numTilesSize = Vector3f(-1, -1, -1));
	~PlayerPhysicsComponent();
	virtual void init();
	void Update();
	void handle_input(Observable* obs);
	void change_state(MovementState* newstate);

private:
	MovementState* _movement_state;
};

#endif // !PLAYER_PHYSICS_COMPONENT
