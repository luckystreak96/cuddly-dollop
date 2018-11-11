#ifndef OBSERVER_H__
#define OBSERVER_H__

#include "observable.h"

#include <memory>

class Observer
{
public:
	Observer();
	virtual void Update(Observable* obs);
	bool _done;
};

#endif // !OBSERVER_H__
