#include "eventManager.h"
#include "gameData.h"

std::vector<std::shared_ptr<IEvent>> EventManager::m_locks = std::vector<std::shared_ptr<IEvent>>();

EventManager::EventManager() : m_queues(std::vector<std::shared_ptr<EventQueue>>())
{

}

EventManager::~EventManager()
{
	m_locks.clear();
	UpdateLockLevel();
}

void EventManager::Update(double elapsedTime)
{
	for (size_t k = 0; k < m_queues.size(); k++)
	{
		std::shared_ptr<EventQueue> temp = std::shared_ptr<EventQueue>(new EventQueue(-69));
		std::shared_ptr<EventQueue> q = m_queues.at(k);
		// If the queue is empty, dont bother
		if (!q->Count())
		{
			Erase(k);
			k--;
			continue;
		}

		// By default, it assumes every event is done
		// This variable is set to false if one of the events on the way to and/or the blocking one is NOT done
		bool allDone = true;

		// Keep track of i to use it after the loop
		unsigned int i = 0;

		if (GameData::RespectsCondition(q))
		{

			// Iterate through the queue until a blocking event is hit
			for (i; i < q->Count(); i++)
			{
				// If the event is completed, dont bother looking at it
				if (!q->Get(i)->IsCompleted())
				{
					// Add to lock vector if the level is > 0
					AddToLockVector(q->Get(i));

					// The eventexecutionmode determines the end of the loop -> when its blocking
					EventExecutionMode eem = q->Get(i)->GetExecutionMode();

					// Is the event done?
					EventUpdateResponse eur = q->Get(i)->UpdateEvent(elapsedTime, m_entities);
					if (!eur.IsDone)
						allDone = false;
					else
						RemoveLock(q->Get(i));

					if (eur.Queue && eur.Queue->Count() > 0)
					{
						temp = eur.Queue;
					}

					// Loop breaks to handle stuff
					if (eem == EventExecutionMode::BLOCKING && allDone == false)
					{
						// Update is done
						break;
					}
				}
			}

			// The order is i++, then check the condition, so it needs to be decremented
			if (i >= q->Count())
				i--;

			// If they are all done, remove the queue
			if (allDone)
			{
				// If repeat is on, send it to the back of the queue
				if (q->IsRepeating())
				{
					for (unsigned int i = 0; i < q->Count(); i++)
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
					// Remove the queue if necessary
					Erase(k);
				}

				// If the queue is done, set k to continue
				if (q && !q->IsRepeating() && q->IsDone())
					k--;
			}

			// Add the returned queue
			if (temp->GetID() != -69)
			{
				PushBack(temp);
			}
		}
	}
}

void EventManager::PushBack(std::shared_ptr<EventQueue> ev)
{
	bool found = false;

	for (auto x : m_queues)
		if (ev->GetID() != -1 && x->GetID() == ev->GetID())
			found = true;

	if (ev->Flag == "" || GameData::RespectsCondition(ev) || ev->GetActivationType() == AT_Autorun)
	{
		if (!found && ev->GetID() != -1)
		{
			for (unsigned int i = 0; i < ev->Count(); i++)
				ev->Get(i)->ResetEvent();
			m_queues.push_back(ev);
		}
		else if (ev->GetID() == -1)
		{
			m_queues.push_back(ev->Clone());
		}
	}
}

void EventManager::RemoveLock(std::shared_ptr<IEvent> ev)
{
	size_t i = 0;
	for (i; i < m_locks.size(); i++)
		if (m_locks.at(i).get() == ev.get())
			break;
	if (i < m_locks.size())
		m_locks.erase(m_locks.begin() + i);
}

void EventManager::Erase(unsigned int queueIndex)
{
	// Roundabout way of destroying the event in the lock vector and the queue
	std::shared_ptr<EventQueue> q = m_queues.at(queueIndex);
	// If the specified q is empty, just destroy it and dont ask any questions
	if (!q->Count())
	{
		m_queues.erase(m_queues.begin() + queueIndex);
		return;
	}

	//Remove all events from the lock
	for (unsigned int e = 0; e < q->Count(); e++)
	{
		std::shared_ptr<IEvent> ev = q->Get(e);
		RemoveLock(ev);
	}
	//q->Remove(index);

	if (IsAllDone(queueIndex))
	{
		m_queues.erase(m_queues.begin() + queueIndex);
	}

	UpdateLockLevel();
}

bool EventManager::IsAllDone(unsigned int index)
{
	bool allDone = true;
	if (index >= m_queues.size())
		return true;
	std::shared_ptr<EventQueue> q = m_queues.at(index);
	return q->IsDone();


	return allDone;
}


// Lock the input to the highest event
void EventManager::UpdateLockLevel()
{
	unsigned int i = 0;
	for (auto x : m_locks)
		i = x->GetLockLevel() > i ? x->GetLockLevel() : i;

	InputManager::GetInstance().PopLockLevel();
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

void EventManager::force_push_front(std::shared_ptr<EventQueue> ev)
{
	if (ev->GetID() != -1)
	{
		for (unsigned int i = 0; i < ev->Count(); i++)
			ev->Get(i)->ResetEvent();
		m_queues.push_back(ev);
	}
	else
	{
		m_queues.push_back(ev->Clone());
	}
}

