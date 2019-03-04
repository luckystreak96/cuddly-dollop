#include "movementStates.h"

#include "playerPhysicsComponent.h"

MovementStateDashing::MovementStateDashing()
{
	_duration_counter = 0;
	_speed_multiplier = 8.0f;
}

void MovementStateDashing::enter(PlayerPhysicsComponent& ppc)
{
	Vector3f vel = ppc.Velocity();
	vel *= _speed_multiplier;

	ppc.set_velocity(vel);
}

void MovementStateDashing::exit(PlayerPhysicsComponent & ppc)
{
	Vector3f vel = ppc.Velocity();
	vel /= _speed_multiplier;

	ppc.set_velocity(vel);
}

MovementState* MovementStateDashing::update(PlayerPhysicsComponent& ppc)
{
	_duration_counter++;
	if (_duration_counter > 7)
		return new MovementStateNormal();
	return nullptr;
}

MovementState* MovementStateDashing::handle_input(PlayerPhysicsComponent& ppc, Observable* obs)
{
	return nullptr;
}

void MovementStateNormal::enter(PlayerPhysicsComponent& ppc)
{
}

MovementState* MovementStateNormal::update(PlayerPhysicsComponent& ppc)
{
	ppc.apply_natural_deceleration();
	return nullptr;
}

MovementState* MovementStateNormal::handle_input(PlayerPhysicsComponent& ppc, Observable* obs)
{
	if (((InputManager*)obs)->FrameKeyStatus(A_Accept, KeyStatus::KeyPressed))
		return new MovementStateDashing();

	InputManager* man = (InputManager*)obs;
	bool up = man->FrameKeyStatus(A_Up);
	bool down = man->FrameKeyStatus(A_Down);
	bool left = man->FrameKeyStatus(A_Left);
	bool right = man->FrameKeyStatus(A_Right);

	ppc.ActionMove(up, down, left, right, InputManager::GetInstance().GetKeyPercent(A_Left), InputManager::GetInstance().GetKeyPercent(A_Down));

	return nullptr;
}
