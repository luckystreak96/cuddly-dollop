#ifndef COMMAND_LINE_H__
#define COMMAND_LINE_H__

#include <string>
#include "command.h"

class CommandLine
{
public:
	CommandLine();
	void handle_input();

private:
	std::string m_buffer;
};

#endif