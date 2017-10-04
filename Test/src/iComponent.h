#ifndef ICOMPONENT_H__
#define ICOMPONENT_H__

#include <string>
#include <vector>
#include "vector3f.h"

enum CMsg {Update};

class IComponent
{
public:
	virtual void ReceiveMessage(std::vector<std::string> message) {};
};

#endif
