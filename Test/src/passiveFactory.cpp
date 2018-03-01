#include "passiveFactory.h"

#include <algorithm>
#include <map>
#include "actorFactory.h"
#include "statCurve.h"

void PassiveFactory::ApplyAllPassives(Fighter_ptr fighter, std::vector<Passive_ptr>* passives)
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
			if (std::find_if(fighter->Skills.begin(), fighter->Skills.end(), [=](Skill_ptr p) {return p->_name == passive->_Data._String; }) == fighter->Skills.end())
				fighter->Skills.push_back(ActorFactory::BuildSkill(passive->_Data._String));
	}
	//ApplySkillPassives(fighter, typeVectors.at(PT_Skill));

	if (typeVectors.count(PassiveType::PT_SkillUpgrade))
		ApplySkillUpgradePassives(fighter, typeVectors.at(PT_SkillUpgrade));
}

// Make sure to reset the stats before calling this
void PassiveFactory::ApplyStatPassives(Fighter_ptr fighter, std::vector<Passive_ptr> passives)
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
		Stat* stat = StatCurve::GetFullStatPointer(x.first, fighter.get());
		stat->Real += x.second.flatTotal;
		stat->Real *= x.second.percentTotal;
	}
}

// Make sure to reset the skill before calling this
void PassiveFactory::ApplySkillUpgradePassives(Fighter_ptr fighter, std::vector<Passive_ptr> passives)
{
	for (auto passive : passives)
	{
		std::vector<Skill_ptr>::iterator skill = std::find_if(fighter->Skills.begin(), fighter->Skills.end(),
			[=](Skill_ptr p) { return p->_name == passive->_Data._String; }
		);

		if (skill != fighter->Skills.end())
			if ((*skill)->ApplySkillUpgrade())
				fighter->SkillPoints -= 1;
	}
}
