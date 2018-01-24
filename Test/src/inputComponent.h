#ifndef INPUT_COMPONENT_H__
#define INPUT_COMPONENT_H__

#include <iostream>
#include "iComponent.h"
#include "input_manager.h"
#include "elapsedTime.h"

class InputComponent : public IComponent
{
public:
	InputComponent();
	virtual void Update();
};

#endif
