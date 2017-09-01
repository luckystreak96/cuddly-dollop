#ifndef INPUT_COMPONENT_H__
#define INPUT_COMPONENT_H__

#include "iComponent.h"
#include <iostream>

class InputComponent : public IComponent
{
public:
	void ReceiveMessage(std::string message) {};

	virtual void Update() {};
	virtual void Interact() { static int counter = 0; std::cout << "Wow ur so cool! " << counter << std::endl; }
};

#endif
