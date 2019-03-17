//
// Created by yanik on 16/03/19.
//

#include "event_command_line.h"

#include "eventQueue.h"
#include "event_command_factory.h"

EventCommandLine::EventCommandLine(EventManager *evtMngr, MapHandler* mapHandler)
{
    m_event_manager = evtMngr;
    m_map_handler = mapHandler;
}

GraphicsComponent *EventCommandLine::get_graphics()
{
    return m_graphics.get();
}

void EventCommandLine::create_events(CommandParams params)
{
    std::shared_ptr<EventQueue> events = EventCommandFactory::create_events(params, m_map_handler);
    m_event_manager->force_push_front(events);

    m_cur_command = CommandParams();
}

EventCommandLine::EventCommandLine() {

}

void EventCommandLine::handle_input(char c) {
    CommandLine::handle_input(c);
    if(m_cur_command._command != "")
        create_events(m_cur_command);
}
