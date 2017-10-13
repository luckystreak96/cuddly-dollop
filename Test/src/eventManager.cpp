#include "eventManager.h"

std::vector<IEvent*> EventManager::m_locks = std::vector<IEvent*>();

EventManager::EventManager() : m_queues(std::vector<EventQueue>())
{

}

void EventManager::Update(double elapsedTime)
{
	for (int k = 0; k < m_queues.size(); k++)
	{
		EventQueue& q = m_queues.at(k);
		// If the queue is empty, dont bother
		if (!q.Count())
			return;

		// By default, it assumes every event is done
		// This variable is set to false if one of the events on the way to and/or the blocking one is NOT done
		bool allDone = true;

		// Keep track of i to use it after the loop
		int i = 0;

		// Iterate through the queue until a blocking event is hit
		for (i; i < q.Count(); i++)
		{
			// Add to lock vector if the level is > 0
			AddToLockVector(q.Get(i));

			// The eventexecutionmode determines the end of the loop -> when its blocking
			EventExecutionMode eem = q.Get(i)->GetExecutionMode();

			// Is the event done?
			EventUpdateResponse eur = q.Get(i)->UpdateEvent(elapsedTime, m_entities);
			if (!eur.IsDone)
				allDone = false;

			if (eur.Queue.Count() > 0)
				PushBack(eur.Queue);

			// Loop breaks to handle stuff
			if (eem == EventExecutionMode::BLOCKING)
			{
				// Update is done
				break;
			}
		}
		// The order is i++, then check the condition, so it needs to be decremented
		if (i >= q.Count())
			i--;

		// If they are all done, remove the specific events
		// Re-call this function to start the next ones
		if (allDone)
		{
			// If repeat is on, send it to the back of the queue
			if (q.IsRepeating())
			{
				for (int i = 0; i < q.Count(); i++)
				{
					if (q.Get(0)->GetExecutionMode() == BLOCKING)
					{
						q.Get(0)->ResetEvent();
						q.SendToBack();
						break;
					}
					q.Get(0)->ResetEvent();
					q.SendToBack();
				}
			}
			else
			{
				// Remove the events from the queue
				for (int j = i; j >= 0; j--)
					Erase(j, k);
			}

			// If the queue is not done, continue updates
			if (q.Count())
				Update(elapsedTime);
		}
	}
}

void EventManager::PushBack(EventQueue ev)
{
	m_queues.push_back(ev);
}

void EventManager::Erase(unsigned int index, unsigned int queueIndex)
{
	// Roundabout way of destroying the event in the lock vector and the queue
	EventQueue& q = m_queues.at(queueIndex);
	IEvent* ev = q.Get(index);

	int i = 0;
	for (i; i < m_locks.size(); i++)
		if (m_locks.at(i) == q.Get(index))
			break;
	m_locks.erase(m_locks.begin() + i);
	q.Remove(index);

	if (!q.Count())
		m_queues.erase(m_queues.begin() + queueIndex);

	delete ev;

	UpdateLockLevel();
}

// Lock the input to the highest event
void EventManager::UpdateLockLevel()
{
	unsigned int i = 0;
	for (auto x : m_locks)
		i = x->GetLockLevel() > i ? x->GetLockLevel() : i;

	InputManager::GetInstance().SetLockLevel(i);
}

void EventManager::AddToLockVector(IEvent* ev)
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

void EventManager::SetEntitiesMap(std::map<unsigned int, Entity*>* ents)
{
	m_entities = ents;
}

