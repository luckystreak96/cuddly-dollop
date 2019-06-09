#include "command_line.h"

#include "utils.h"
#include <algorithm>

CommandParams::CommandParams()
{
    _command = "";
    _args = std::vector<std::string>();
}

CommandLine::CommandLine()
{
    m_buffer = "";
    m_cur_command = CommandParams();
}

bool CommandLine::handle_input(char c)
{
    if(m_buffer.empty() && c != '/')
        return false;

    if(c == '\n')
    {
        m_cur_command = create_command();
        m_buffer = "";
        return true;
    }
    else if (c == '\b')
    {
        m_buffer.pop_back();
        if(m_buffer.empty())
            return true;
    }
    else
        m_buffer += c;

//    std::cout << m_buffer << std::endl;
    return false;
}

CommandParams CommandLine::get_command()
{
    return m_cur_command;
}

// Tries to create the command based on the buffer
CommandParams CommandLine::create_command()
{
    std::string command;
    std::vector<std::string> args;

    //Split the string
    std::vector<std::string> temp = Utils::Split(m_buffer, '"');
    std::vector<std::string> split_str;

    for(int i = 0; i < temp.size(); i++) {
        if (i % 2 != 0)
            split_str.push_back(temp[i]);
        else
        {
            std::vector<std::string> strs = Utils::Split(temp[i], ' ');
            split_str.insert(split_str.end(), strs.begin(), strs.end());
        }
    }

    // Remove empty strings
    split_str.erase(std::remove(split_str.begin(), split_str.end(), ""), split_str.end());

    // If it's empty, return nothing
    if(split_str.size() <= 0)
        return CommandParams();

    command = split_str[0].substr(1);

    for(int i = 1; i < split_str.size(); i++)
        args.push_back(split_str[i]);

    CommandParams result;
    result._command = command;
    result._args = args;

    return result;
}

bool CommandLine::is_reading() {
    return !m_buffer.empty();
}
