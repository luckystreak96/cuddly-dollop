#ifndef INPUT_COMPONENT_H__
#define INPUT_COMPONENT_H__

#include "iComponent.h"
#include <iostream>

class InputComponent : public IComponent
{
public:
	void ReceiveMessage(std::vector<std::string> message) {};

	virtual void Update() {};
	virtual std::vector<std::string> Interact() { return std::vector<std::string>() = {"TELEPORT", "6", "6", "4"}; }
};

#endif
