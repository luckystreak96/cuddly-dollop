#include "eventQueue.h"

std::vector<IEvent*> EventQueue::m_locks = std::vector<IEvent*>();

EventQueue::EventQueue() : m_queue(std::deque<IEvent*>())
{

}

void EventQueue::Update(double elapsedTime)
{
	// If the queue is empty, dont bother
	if (Complete())
		return;

	// By default, it assumes every event is done
	// This variable is set to false if one of the events on the way to and/or the blocking one is NOT done
	bool allDone = true;

	// Keep track of i to use it after the loop
	int i = 0;

	// Iterate through the queue until a blocking event is hit
	for (i; i < m_queue.size(); i++)
	{
		// Add to lock vector if the level is > 0
		AddToLockVector(m_queue.at(i));

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
	// The order is i++, then check the condition, so it needs to be decremented
	if (i >= m_queue.size())
		i--;

	// If they are all done, remove the specific events
	// Re-call this function to start the next ones
	if (allDone)
	{
		// Remove the events from the queue
		for (int j = i; j >= 0; j--)
			Erase(j);

		// If the queue is not done, continue updates
		if (!Complete())
			Update(elapsedTime);
	}
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
	// Roundabout way of destroying the event in the lock vector and the queue

	IEvent* ev = m_queue.at(index);

	int i = 0;
	for (i; i < m_locks.size(); i++)
		if (m_locks.at(i) == m_queue.at(index))
			break;
	m_locks.erase(m_locks.begin() + i);
	m_queue.erase(m_queue.begin() + index);

	if (ev)
		delete ev;

	UpdateLockLevel();
}

// Lock the input to the highest event
void EventQueue::UpdateLockLevel()
{
	unsigned int i = 0;
	for (auto x : m_locks)
		i = x->GetLockLevel() > i ? x->GetLockLevel() : i;

	InputManager::GetInstance().SetLockLevel(i);
}

void EventQueue::AddToLockVector(IEvent* ev)
{
	// The event needs to have a lock level greater than 0
	if (ev->GetLockLevel() < 0)
		return;

	// If the event is already in the list, just return
	if (std::find(m_locks.begin(), m_locks.end(), ev) != m_locks.end())
		return;

	// Add to the list and update lock level
	m_locks.push_back(ev);
	UpdateLockLevel();
}