#ifndef COMMAND_LINE_H__
#define COMMAND_LINE_H__

#include <vector>
#include <string>
#include "command.h"

struct CommandParams
{
	CommandParams();

	std::string _command;
	std::vector<std::string> _args;
};

// Handles the input string and separates the commands from the args
class CommandLine
{
public:
	CommandLine();
	virtual bool handle_input(char c);

	// Returns a CommandParam
	// The _command will be an empty string if the command doesn't exist
	CommandParams get_command();
	bool is_reading();

protected:
	// Tries to create the command based on the buffer
	CommandParams create_command();

protected:
	std::string m_buffer;
	CommandParams m_cur_command;
};

#endif