#include "eventQueue.h"

std::vector<IEvent*> EventQueue::m_locks = std::vector<IEvent*>();

EventQueue::EventQueue() : m_queue(std::deque<IEvent*>())
{

}

void EventQueue::Update(double elapsedTime)
{
	// By default, it assumes every event is done
	// This variable is set to false if one of the events on the way to and/or the blocking one is NOT done
	bool allDone = true;

	// Keep track of i to use it after the loop
	int i = 0;

	// Iterate through the queue until a blocking event is hit
	for (i; i < m_queue.size(); i++)
	{
		// The eventexecutionmode determines the end of the loop -> when its blocking
		EventExecutionMode eem = m_queue.at(i)->GetExecutionMode();

		// Is the event done?
		if (m_queue.at(i)->UpdateEvent(elapsedTime) == false)
			allDone = false;

		// Loop breaks to handle stuff
		if (eem == EventExecutionMode::BLOCKING)
		{
			// Update is done
			break;
		}
	}

	// If they are all done, remove the specific events
	// Re-call this function to start the next ones
	if (allDone)
	{
		//TODO: remove the events from the queue

		// If the queue is not done, continue updates
		if (!Complete())
			Update(elapsedTime);
	}
}

// Set the queue as complete and unlock input
void EventQueue::UnlockInput()
{
	InputManager::GetInstance().SetLockLevel(0);
}

bool EventQueue::Complete()
{
	return m_queue.empty();
}

void EventQueue::PushBack(IEvent* ev)
{
	m_queue.push_back(ev);
}

void EventQueue::Erase(unsigned int index)
{
	
	m_queue.erase(m_queue.begin() + index);
}
