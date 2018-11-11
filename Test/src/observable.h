#ifndef OBSERVABLE_H__
#define OBSERVABLE_H__

#include <vector>
//#include "observer.h"

class Observer;

class Observable
{
public:
	virtual void UpdateObservers();
	void AddObserver(Observer* obs);

public:
	std::vector<Observer*> _observers;
};

#endif // !OBSERVABLE_H__
