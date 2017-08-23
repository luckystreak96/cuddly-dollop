#ifndef ICOMPONENT_H__
#define ICOMPONENT_H__

#include <string>

enum CMsg {Update};

class IComponent
{
private:
	const std::string MESSAGE_TOKEN = "#";

public:
	virtual void ReceiveMessage(std::string message) {};

};

#endif
