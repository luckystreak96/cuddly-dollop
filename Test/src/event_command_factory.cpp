//
// Created by yanik on 16/03/19.
//

#include "event_command_factory.h"

#include "eventFactory.h"

std::shared_ptr<EventQueue> EventCommandFactory::create_events(CommandParams cp, MapHandler* mapHandler)
{
    std::shared_ptr<EventQueue> queue = std::shared_ptr<EventQueue>(new EventQueue(-1));

    std::string command = cp._command;
    std::map<std::string, EventArgType> eventArgs;

    // Set attributes
    // (Probably unnecessary)
    queue->Flag = "";
    queue->FlagValue = 0;
    queue->Condition = FlagCondition::FC_Value;
    queue->SetActivationType(ActivationType::AT_Autorun);

    // Insert events
    EventTypes eventType = EventTypes::ET_DialogueBox;

    if (EventFactory::TypeDict.find(cp._command) == EventFactory::TypeDict.end())
    {
        std::cout << "ERROR - Command does not exist." << std::endl;
        return queue;
    }

    EventTypeInfo info = EventFactory::TypeDict[command];
    eventType = info._eventType;

    for (int i = 0; i < cp._args.size(); i++)
    {
        std::string title = "free param " + std::to_string(i);
        if(info._args.size() <= i)
            eventArgs.emplace(title, EventArgType::str_to_eai(cp._args[i]));
        else
            eventArgs.emplace(info._args[i].first, EventArgType::str_to_eai(cp._args[i]));
    }


    std::shared_ptr<IEvent> event = EventFactory::BuildEvent(eventType, eventArgs, mapHandler);
    queue->PushBack(event);

    return queue;
}
