//
// Created by yanik on 08/03/19.
//

#include "command.h"

#ifndef CUDDLY_DOLLOP_MUTE_BGM_COMMAND_H
#define CUDDLY_DOLLOP_MUTE_BGM_COMMAND_H

class MuteBGMCommand : public Command
{
public:
    MuteBGMCommand();
    void execute();
private:
};

#endif //CUDDLY_DOLLOP_MUTE_BGM_COMMAND_H
