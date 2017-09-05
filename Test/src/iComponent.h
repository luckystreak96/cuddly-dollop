#ifndef ICOMPONENT_H__
#define ICOMPONENT_H__

#include <string>
#include <vector>

enum CMsg {Update};

class IComponent
{
public:
	virtual void ReceiveMessage(std::vector<std::string> message) {};

};

#endif
