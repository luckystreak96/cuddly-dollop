//
// Created by yanik on 16/03/19.
//

#include "event_command_line.h"

#include "eventQueue.h"
#include "event_command_factory.h"
#include "fontManager.h"

#include <limits>
#include <utility>
#include <algorithm>

EventCommandLine::EventCommandLine(EventManager *evtMngr, MapHandler* mapHandler)
{
    m_event_manager = evtMngr;
    m_map_handler = mapHandler;
    init_font();
}

void EventCommandLine::create_events(CommandParams params)
{
    std::shared_ptr<EventQueue> events = EventCommandFactory::create_events(params, m_map_handler);
    m_event_manager->force_push_front(events);

    m_cur_command = CommandParams();
}

EventCommandLine::EventCommandLine() {
    m_font = std::numeric_limits<unsigned int>::max();
}

bool EventCommandLine::handle_input(char c) {
    bool finalize = CommandLine::handle_input(c);
    if(m_cur_command._command != "" || finalize)
        create_events(m_cur_command);

    FontManager::GetInstance().UpdateText(m_font, m_buffer);

    return finalize;
}

EventCommandLine::~EventCommandLine() {
    if(m_font != std::numeric_limits<unsigned int>::max())
        FontManager::GetInstance().RemoveFont(m_font);
}

// move constructor
EventCommandLine::EventCommandLine(EventCommandLine &&other) : m_event_manager(other.m_event_manager), m_map_handler(other.m_map_handler), m_font(other.m_font), m_font_position(other.m_font_position) {
    other.m_font = std::numeric_limits<unsigned int>::max();
}

// copy constructor
EventCommandLine::EventCommandLine(const EventCommandLine &other) : EventCommandLine(other.m_event_manager, other.m_map_handler) {
}

// operator=
EventCommandLine &EventCommandLine::operator=(const EventCommandLine &other) {
    m_event_manager = other.m_event_manager;
    m_map_handler = other.m_map_handler;
    init_font();
    return *this;
}

void EventCommandLine::init_font() {
    m_font_position = Vector3f(0, OrthoProjInfo::GetRegularInstance().Top * 2.f - 32.0f, -10);
    m_font = FontManager::GetInstance().AddFont(true, false, true, "res/fonts/lowercase.png");
    FontManager::GetInstance().SetScale(m_font, 0.25f, 0.25f);
    FontManager::GetInstance().GetFont(m_font)->_letterSpacing = 48.f;
//    FontManager::GetInstance().EnableFont(m_font);
    FontManager::GetInstance().SetText(m_font, " ", m_font_position);
}

EventCommandLine &EventCommandLine::operator=(EventCommandLine &&other) {
    m_event_manager = other.m_event_manager;
    m_map_handler = other.m_map_handler;
    m_font = other.m_font;
    other.m_font = std::numeric_limits<unsigned int>::max();
    m_font_position = other.m_font_position;
    return *this;
}
