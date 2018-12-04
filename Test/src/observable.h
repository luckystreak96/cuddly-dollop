#ifndef OBSERVABLE_H__
#define OBSERVABLE_H__

#include <vector>
#include <memory>
//#include "observer.h"

class Observer;
using Obs_ptr = std::shared_ptr<Observer>;

class Observable
{
public:
	virtual void UpdateObservers();
	void AddObserver(Obs_ptr obs);

public:
	std::vector<Obs_ptr> _observers;
};

#endif // !OBSERVABLE_H__
