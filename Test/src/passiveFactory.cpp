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

	if(typeVectors.count(PT_Stat))
		ApplyStatPassives(fighter, typeVectors.at(PT_Stat));

	// Iterate through all passives
	for (auto& passive : *passives)
	{

		switch (type)
		{
		case PT_Skill:
			break;
		case PT_Stat:
			break;
		case PT_Special:
			break;
		default:
			break;
		}

	}
		//switch (type)
		//{
		//case PT_Skill:
		//	// if it doesnt find a skill with the same name in the skill list, it adds the skill
		//	if (std::find(fighter->Skills.begin(), fighter->Skills.end(), [=](Skill_ptr p) {return p->_name == passive->_Data._String; }) == fighter->Skills.end())
		//		fighter->Skills.push_back(ActorFactory::BuildSkill(passive->_Data._String));
		//	break;
		//case PT_Stat:
		//	break;
		//case PT_Special:
		//	break;
		//default:
		//	break;
		//}
}

void PassiveFactory::ApplyStatPassives(Fighter_ptr fighter, std::vector<Passive_ptr> passives)
{
	// Sort the skills by priority
	//std::sort(passives.begin(), passives.end(), [](Passive_ptr p1, Passive_ptr p2) { return p1->_Priority < p2->_Priority; });

	struct StatBonuses
	{
		float flatTotal, percentTotal = 0;
	};

	std::map<std::string, StatBonuses> bonuses;

	for (auto& x : passives)
	{
		if (!bonuses.count(x->_Data._String))
			bonuses.emplace(x->_Data._String);

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
