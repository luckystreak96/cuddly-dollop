#ifndef INPUT_COMPONENT_H__
#define INPUT_COMPONENT_H__

#include "iComponent.h"
#include <iostream>
#include "input_manager.h"
#include "elapsedTime.h"

class InputComponent : public IComponent
{
public:
	InputComponent();
	void ReceiveMessage(std::vector<std::string> message);

	virtual void Update();
	//virtual std::vector<std::string> Interact(EventQueue* eq, unsigned int parent_id);
private:
	Vector3f m_pos;
	//EventQueue m_eventQueue;
	//const std::string DATA_FILE = "res/data/data.json";
};

#endif
