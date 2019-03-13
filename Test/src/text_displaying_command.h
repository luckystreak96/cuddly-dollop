//
// Created by yanik on 12/03/19.
//

#ifndef PROJECT_TEXT_DISPLAYING_COMMAND_H
#define PROJECT_TEXT_DISPLAYING_COMMAND_H

#include "command.h"
#include <string>

class TextDisplayingCommand : public Command
{
public:
    TextDisplayingCommand(std::string* str);
    virtual std::string get_current_text_state();
protected:
    std::string* m_text;
};

#endif //PROJECT_TEXT_DISPLAYING_COMMAND_H
