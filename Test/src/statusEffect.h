#ifndef STATUS_EFFECT_H__
#define STATUS_EFFECT_H__

#include "passiveSkill.h"
#include "triple.h"
//#include "passive_action.h"
//#include "skill.h"

#include <vector>
#include <tuple>

enum Status { Status_Determined, Status_Pragmatic, Status_Special, Status_Protected, Status_Last };
enum SkillElement { SE_Determined, SE_StrongWilled, SE_Pragmatic, SE_None };
enum TriggerCondition { TC_None, TC_Targeted };

class StatusEffect
{
public:
	StatusEffect() : _Element(SE_None), _Triggered(false) { _Id = counter++; }
	std::vector<std::tuple<Passive_ptr, int>> _Effects;
	std::vector<std::pair<SkillProgress, triple>> _Actions;
	std::string _Name;
	std::string _Sprite;
	int _Applier;
	unsigned int _Id;
	SkillElement _Element;
	TriggerCondition _Condition;
	bool _Triggered;

	static unsigned int counter;
};

#endif