#ifndef INPUT_COMPONENT_H__
#define INPUT_COMPONENT_H__

#include "iComponent.h"

class InputComponent : public IComponent
{
public:
	void ReceiveMessage(std::string message) {};

	virtual void Update();
};

#endif
