#include "observable.h"

#include "observer.h"

void Observable::UpdateObservers()
{
	std::vector<int> to_remove;
	int i = 0;
	for (auto& x : _observers)
	{
		if (x->_done)
			to_remove.push_back(i);
		else
			x->Update(this);

		i++;
	}

	for (int prog = to_remove.size() - 1; prog >= 0; prog--)
		_observers.erase(_observers.begin() + to_remove[prog]);
}

void Observable::AddObserver(Observer* obs)
{
	_observers.push_back(obs);
}
