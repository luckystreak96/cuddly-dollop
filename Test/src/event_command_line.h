//
// Created by yanik on 16/03/19.
//

#ifndef PROJECT_EVENT_COMMAND_LINE_H
#define PROJECT_EVENT_COMMAND_LINE_H

#include "eventManager.h"
#include "command_line.h"
#include "fontGraphicsComponent.h"
#include "map_handler.h"

class EventCommandLine: public CommandLine
{
public:
    EventCommandLine();
    EventCommandLine(EventManager* evtMngr, MapHandler* mapHandler);
    GraphicsComponent* get_graphics();
    void create_events(CommandParams params);
    void handle_input(char c);

private:
    EventManager* m_event_manager;
    MapHandler* m_map_handler;
    std::shared_ptr<FontGraphicsComponent> m_graphics;
};

#endif //PROJECT_EVENT_COMMAND_LINE_H
