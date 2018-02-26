#ifndef PASSIVE_FACTORY_H__
#define PASSIVE_FACTORY_H__

#include "fighter.h"

class PassiveFactory
{
public:
	// Make sure to reset the stats before calling this
	static void ApplyAllPassives(Fighter_ptr fighter, std::vector<Passive_ptr>* passive);

	// Make sure to reset the stats before calling this
	static void ApplyStatPassives(Fighter_ptr fighter, std::vector<Passive_ptr> passive);
};

#endif