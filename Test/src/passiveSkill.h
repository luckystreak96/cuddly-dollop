#ifndef PASSIVE_SKILL_H__
#define PASSIVE_SKILL_H__

#include <string>
#include <memory>

enum PassivePriority { PP_AbsoluteFirst, PP_Secondary, PP_Last, PP_BeforeSkill, PP_BeforeDamage, PP_AfterDamage, PP_AfterSkill };
enum PassiveType { PT_Skill, PT_Stat, PT_Special };
enum PassiveSpecifier { PS_Flat, PS_Percent, PS_None };

struct PassiveData
{
	PassiveData();
	std::string _String;
	int _Integer;
	float _Float;
};

class PassiveSkill;
typedef std::shared_ptr<PassiveSkill> Passive_ptr;

struct PassiveSkill
{
	PassiveSkill();

	std::string _Name;
	PassivePriority _Priority;
	PassiveType _Type;
	PassiveSpecifier _Specifier;
	PassiveData _Data;
};

#endif