#ifndef INPUT_COMPONENT_H__
#define INPUT_COMPONENT_H__

#include <iostream>
#include "iComponent.h"
#include "input_manager.h"
#include "elapsedTime.h"
#include "observer.h"

class InputComponent : public IComponent, public Observer
{
public:
	InputComponent();
	virtual void Update();
	void Update(Observable* obs);
};

#endif
