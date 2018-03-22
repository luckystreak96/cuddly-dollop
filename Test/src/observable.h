#ifndef OBSERVABLE_H__
#define OBSERVABLE_H__

#include <vector>
#include "observer.h"

class Observable
{
public:
	virtual void UpdateObservers();

public:
	std::vector<Obs_ptr> _observers;
};

#endif // !OBSERVABLE_H__
