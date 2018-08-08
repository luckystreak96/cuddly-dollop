#ifndef STATUS_EFFECT_H__
#define STATUS_EFFECT_H__

#include "passiveSkill.h"
//#include "skill.h"

#include <vector>
#include <tuple>

enum StatusList { Determined, Pragmatic };
enum SkillElement { SE_Determined, SE_StrongWilled, SE_Pragmatic, SE_None };

class StatusEffect
{
public:
	StatusEffect() : _Element(SE_None) {}
	std::vector<std::tuple<Passive_ptr, int>> _Effects;
	std::string _Name;
	std::string _Sprite;
	SkillElement _Element;
};

#endif