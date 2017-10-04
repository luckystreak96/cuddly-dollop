#ifndef INPUT_COMPONENT_H__
#define INPUT_COMPONENT_H__

#include "iComponent.h"
#include <iostream>
#include "fontManager.h"
#include "input_manager.h"

class InputComponent : public IComponent
{
public:
	InputComponent();
	void ReceiveMessage(std::vector<std::string> message);

	virtual void Update() {};
	virtual std::vector<std::string> Interact();
private:
	Vector3f m_pos;
};

#endif
