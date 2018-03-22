#ifndef OBSERVER_H__
#define OBSERVER_H__

#include <memory>

class Observer;
typedef std::shared_ptr<Observer> Obs_ptr;

class Observer
{
public:
	Observer();
	virtual void Update();
	bool _done;
};

#endif // !OBSERVER_H__
