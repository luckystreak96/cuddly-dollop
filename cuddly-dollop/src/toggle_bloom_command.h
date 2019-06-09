//
// Created by yanik on 23/03/19.
//

#ifndef PROJECT_TOGGLE_BLOOM_COMMAND_H
#define PROJECT_TOGGLE_BLOOM_COMMAND_H

#include "text_displaying_command.h"

class ToggleBloomCommand : public TextDisplayingCommand
{
public:
    ToggleBloomCommand(std::string* str);
    void execute();
    std::string get_current_text_state();
};

#endif //PROJECT_TOGGLE_BLOOM_COMMAND_H
