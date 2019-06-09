//
// Created by yanik on 08/03/19.
//

#ifndef CUDDLY_DOLLOP_MUTE_BGM_COMMAND_H
#define CUDDLY_DOLLOP_MUTE_BGM_COMMAND_H

#include "text_displaying_command.h"

class MuteBGMCommand : public TextDisplayingCommand
{
public:
    MuteBGMCommand(std::string* str);
    void execute();
    std::string get_current_text_state();
private:
};

#endif //CUDDLY_DOLLOP_MUTE_BGM_COMMAND_H
