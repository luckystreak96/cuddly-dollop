#ifndef PASSIVE_FACTORY_H__
#define PASSIVE_FACTORY_H__

#include "fighter.h"
#include "battleData.h"

class PassiveFactory
{
	friend class Fighter;
public:
	static Passive_ptr BuildPassive(std::string name, PassivePriority priority, PassiveType type, PassiveSpecifier specifier, float fvalue = 0, int ivalue = 0, std::string svalue = "");

	// Make sure to reset the stats before calling this
	static void ApplyAllPassives(Fighter* fighter, std::vector<Passive_ptr>* passive);

	// Make sure to reset the stats before calling this
	static void ApplyStatPassives(Fighter* fighter, std::vector<Passive_ptr> passive);

	// Make sure to reset the skill before calling this
	static void ApplySkillUpgradePassives(Fighter* fighter, std::vector<Passive_ptr> passive);

	//static void ApplySkillPassives(Fighter_ptr fighter, std::vector<Passive_ptr> passive);
};

#endif