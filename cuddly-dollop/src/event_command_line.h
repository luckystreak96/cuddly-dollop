//
// Created by yanik on 16/03/19.
//

#ifndef PROJECT_EVENT_COMMAND_LINE_H
#define PROJECT_EVENT_COMMAND_LINE_H

#include "eventManager.h"
#include "command_line.h"
#include "map_handler.h"

class EventCommandLine: public CommandLine
{
public:
    EventCommandLine();
    EventCommandLine(EventManager* evtMngr, MapHandler* mapHandler);

    // These "big five" are necessary to avoid garbage collecting the font
    ~EventCommandLine();
    EventCommandLine(EventCommandLine&& other);
    EventCommandLine(const EventCommandLine& other);
    EventCommandLine& operator=(const EventCommandLine& other);
    EventCommandLine& operator=(EventCommandLine&& other);

    void init_font();
    void create_events(CommandParams params);
    bool handle_input(char c) override;

private:
    EventManager* m_event_manager;
    MapHandler* m_map_handler;
    Vector3f m_font_position;
    unsigned int m_font;
};

#endif //PROJECT_EVENT_COMMAND_LINE_H
