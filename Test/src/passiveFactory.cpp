#include "passiveFactory.h"

#include <algorithm>
#include <map>
#include "fighterFactory.h"
#include "statCurve.h"

Passive_ptr PassiveFactory::BuildPassive(std::string name, PassivePriority priority, PassiveType type, PassiveSpecifier specifier, float fvalue, int ivalue, std::string svalue)
{
	Passive_ptr result = std::shared_ptr<PassiveSkill>(new PassiveSkill());

	result->_Name = name;
	result->_Priority = priority;
	result->_Specifier = specifier;
	result->_Type = type;
	result->_Data._Float = fvalue;
	result->_Data._Integer = ivalue;
	result->_Data._String = svalue;

	return result;
}


void PassiveFactory::ApplyAllPassives(Fighter* fighter, std::vector<Passive_ptr>* passives)
{
	// Sort the skills by type
	std::sort(passives->begin(), passives->end(), [](Passive_ptr p1, Passive_ptr p2) { return p1->_Type < p2->_Type; });

	std::map<PassiveType, std::vector<Passive_ptr>> typeVectors;

	PassiveType type = (PassiveType)0;
	typeVectors.emplace(type, std::vector<Passive_ptr>());
	for (auto passive : *passives)
	{
		if (passive->_Type != type)
		{
			type = passive->_Type;
			typeVectors.emplace(type, std::vector<Passive_ptr>());
		}

		typeVectors[type].push_back(passive);
	}

	if (typeVectors.count(PT_Stat))
		ApplyStatPassives(fighter, typeVectors.at(PT_Stat));

	if (typeVectors.count(PassiveType::PT_Skill))
	{
		for (auto passive : typeVectors.at(PT_Skill))
			if (std::find_if(fighter->GetSkills().begin(), fighter->GetSkills().end(), [=](Skill_ptr p) {return p->_name == passive->_Data._String; }) == fighter->GetSkills().end())
				fighter->GetSkills().push_back(FighterFactory::BuildSkill(passive->_Data._String));
	}
	//ApplySkillPassives(fighter, typeVectors.at(PT_Skill));

	if (typeVectors.count(PassiveType::PT_SkillUpgrade))
		ApplySkillUpgradePassives(fighter, typeVectors.at(PT_SkillUpgrade));
}

// Make sure to reset the stats before calling this
void PassiveFactory::ApplyStatPassives(Fighter* fighter, std::vector<Passive_ptr> passives)
{
	// Sort the skills by priority
	//std::sort(passives.begin(), passives.end(), [](Passive_ptr p1, Passive_ptr p2) { return p1->_Priority < p2->_Priority; });

	struct StatBonuses
	{
		float flatTotal = 0;
		float percentTotal = 1;
	};

	std::map<std::string, StatBonuses> bonuses;

	for (auto& x : passives)
	{
		if (!bonuses.count(x->_Data._String))
			bonuses.emplace(x->_Data._String, StatBonuses());

		if (x->_Specifier == PS_Percent)
			bonuses[x->_Data._String].percentTotal += x->_Data._Float;
		else
			bonuses[x->_Data._String].flatTotal += x->_Data._Float;
	}

	for (auto& x : bonuses)
	{
		Stat* stat = StatCurve::GetFullStatPointer(x.first, fighter);
		stat->Real += x.second.flatTotal;
		stat->Real *= x.second.percentTotal;
		stat->Modified = stat->Real;
	}
}

// Make sure to reset the skill before calling this
void PassiveFactory::ApplySkillUpgradePassives(Fighter* fighter, std::vector<Passive_ptr> passives)
{
	for (auto passive : passives)
	{
		std::vector<Skill_ptr>::iterator skill = std::find_if(fighter->GetSkills().begin(), fighter->GetSkills().end(),
			[=](Skill_ptr p) { return p->_name == passive->_Data._String; }
		);

		if (skill != fighter->GetSkills().end())
			if ((*skill)->ApplySkillUpgrade())
				fighter->SkillPoints -= 1;
	}
}
