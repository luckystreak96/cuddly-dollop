#include "observable.h"

void Observable::UpdateObservers()
{
	std::vector<Obs_ptr> to_remove;
	for (auto& x : _observers)
	{
		if (x->_done)
			to_remove.push_back(x);
		else
			x->Update();
	}

	for (auto x : to_remove)
		_observers.erase(std::find(_observers.begin(), _observers.end(), x));
}