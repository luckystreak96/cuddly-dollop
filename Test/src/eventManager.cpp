#include "eventManager.h"

std::vector<std::shared_ptr<IEvent>> EventManager::m_locks = std::vector<std::shared_ptr<IEvent>>();

EventManager::EventManager() : m_queues(std::vector<std::shared_ptr<EventQueue>>())
{

}

void EventManager::Update(double elapsedTime)
{
	for (int k = 0; k < m_queues.size(); k++)
	{
		std::shared_ptr<EventQueue> temp = std::shared_ptr<EventQueue>(new EventQueue(-69));
		std::shared_ptr<EventQueue>& q = m_queues.at(k);
		// If the queue is empty, dont bother
		if (!q->Count())
		{
			Erase(0, k);
			continue;
		}

		// By default, it assumes every event is done
		// This variable is set to false if one of the events on the way to and/or the blocking one is NOT done
		bool allDone = true;

		// Keep track of i to use it after the loop
		int i = 0;

		// Iterate through the queue until a blocking event is hit
		for (i; i < q->Count(); i++)
		{
			// Add to lock vector if the level is > 0
			AddToLockVector(q->Get(i));

			// The eventexecutionmode determines the end of the loop -> when its blocking
			EventExecutionMode eem = q->Get(i)->GetExecutionMode();

			// Is the event done?
			EventUpdateResponse eur = q->Get(i)->UpdateEvent(elapsedTime, m_entities);
			if (!eur.IsDone)
				allDone = false;

			if (eur.Queue && eur.Queue->Count() > 0)
			{
				temp = eur.Queue;
			}

			// Loop breaks to handle stuff
			if (eem == EventExecutionMode::BLOCKING)
			{
				// Update is done
				break;
			}
		}
		// The order is i++, then check the condition, so it needs to be decremented
		if (i >= q->Count())
			i--;

		// If they are all done, remove the specific events
		// Re-call this function to start the next ones
		if (allDone)
		{
			// If repeat is on, send it to the back of the queue
			if (q->IsRepeating())
			{
				for (int i = 0; i < q->Count(); i++)
				{
					if (q->Get(0)->GetExecutionMode() == BLOCKING)
					{
						q->Get(0)->ResetEvent();
						q->SendToBack();
						break;
					}
					q->Get(0)->ResetEvent();
					q->SendToBack();
				}
			}
			else
			{
				// Remove the events from the queue
				for (int j = i; j >= 0; j--)
					Erase(j, k);
			}

			// If the queue is not done, continue updates
			// COMMENTING THIS NEXT LINE MAY CAUSE PROBLEMS XDXDXDXDXDXDXDDXDXDXDXDXDXD
			// LIKE AN EVENT RUNNING 2 FRAMES WHEN IT SHOULDNT???
			// IF LOTS OF FRAMES ARE RUNNING PARRALLEL (PLZ NO FCK ME)
			//if (q->Count())
				Update(elapsedTime);
		}
		if (temp->GetID() != -69)
		{
			PushBack(temp);
			Update(elapsedTime);
		}
	}
}

void EventManager::PushBack(std::shared_ptr<EventQueue> ev)
{
	bool found = false;

	for (auto x : m_queues)
		if (ev->GetID() != -1 && x->GetID() == ev->GetID())
			found = true;

	if (!found || ev->GetID() == -1)
		m_queues.push_back(ev);
}

void EventManager::Erase(unsigned int index, unsigned int queueIndex)
{
	// Roundabout way of destroying the event in the lock vector and the queue
	std::shared_ptr<EventQueue> q = m_queues.at(queueIndex);
	// If the specified q is empty, just destroy it and dont ask any questions
	if (!q->Count())
	{
		m_queues.erase(m_queues.begin() + queueIndex);
		return;
	}

	std::shared_ptr<IEvent> ev = q->Get(index);

	int i = 0;
	for (i; i < m_locks.size(); i++)
		if (m_locks.at(i).get() == ev.get())
			break;
	m_locks.erase(m_locks.begin() + i);
	q->Remove(index);

	if (!q->Count())
		m_queues.erase(m_queues.begin() + queueIndex);

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

void EventManager::AddToLockVector(std::shared_ptr<IEvent> ev)
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

void EventManager::SetEntitiesMap(std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	m_entities = ents;
}

