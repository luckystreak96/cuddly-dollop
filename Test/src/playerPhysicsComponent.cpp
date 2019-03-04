#include "playerPhysicsComponent.h"

#include "movementStates.h"

PlayerPhysicsComponent::PlayerPhysicsComponent(Vector3f pos, std::string modelName, Vector3f size, Vector3f numTilesSize) : PhysicsComponent(pos, modelName, size, numTilesSize)
{
	init();
}

PlayerPhysicsComponent::~PlayerPhysicsComponent()
{
	delete _movement_state;
}

void PlayerPhysicsComponent::init()
{
	_movement_state = new MovementStateNormal();
}

void PlayerPhysicsComponent::Update()
{
	PhysicsComponent::Update();

	MovementState* newstate = _movement_state->update(*this);
	change_state(newstate);
}

void PlayerPhysicsComponent::handle_input(Observable* obs)
{
	MovementState* newstate = _movement_state->handle_input(*this, obs);
	change_state(newstate);
}

void PlayerPhysicsComponent::change_state(MovementState * newstate)
{
	if (newstate != nullptr)
	{
		_movement_state->exit(*this);
		delete _movement_state;
		_movement_state = newstate;

		_movement_state->enter(*this);
	}
}

