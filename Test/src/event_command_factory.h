//
// Created by yanik on 16/03/19.
//

#ifndef PROJECT_EVENT_COMMAND_FACTORY_H
#define PROJECT_EVENT_COMMAND_FACTORY_H

#include "command_line.h"
#include "eventQueue.h"
#include "map_handler.h"

class EventCommandFactory
{
public:
    static std::shared_ptr<EventQueue> create_events(CommandParams cp, MapHandler* mapHandler);
};

#endif //PROJECT_EVENT_COMMAND_FACTORY_H
