#ifndef PHYSICS_COMPONENT_H__
#define PHYSICS_COMPONENT_H__

#include "iComponent.h"

class PhysicsComponent : public IComponent
{
public:
	void ReceiveMessage(std::string message) {};
	virtual void Update();
};

#endif
