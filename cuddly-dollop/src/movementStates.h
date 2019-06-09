#ifndef MOVEMENT_STATES_H__
#define MOVEMENT_STATES_H__

#include "input_manager.h"
#include "playerPhysicsComponent.h"

class MovementState {
public:
	virtual void enter(PlayerPhysicsComponent& ppc) = 0;
	virtual void exit(PlayerPhysicsComponent& ppc) {}
	virtual MovementState* update(PlayerPhysicsComponent& ppc) = 0;
	virtual MovementState* handle_input(PlayerPhysicsComponent& ppc, Observable* obs) = 0;
};

class MovementStateDashing : public MovementState
{
public:
	MovementStateDashing();
	void enter(PlayerPhysicsComponent& ppc);
	void exit(PlayerPhysicsComponent& ppc);
	MovementState* update(PlayerPhysicsComponent& ppc);
	MovementState* handle_input(PlayerPhysicsComponent& ppc, Observable* obs);
private:
	int _duration_counter;
	int _speed_multiplier;
};

class MovementStateNormal : public MovementState
{
public:
	MovementStateNormal() {}
	void enter(PlayerPhysicsComponent& ppc);
	MovementState* update(PlayerPhysicsComponent& ppc);
	MovementState* handle_input(PlayerPhysicsComponent& ppc, Observable* obs);
};

#endif