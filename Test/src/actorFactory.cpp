#include "actorFactory.h"
#include "skillSmack.h"
#include "skillHeal.h"
#include "skillProtect.h"
#include "passiveFactory.h"

Actor_ptr ActorFactory::BuildBaseAlly()
{
	Actor_ptr result = Actor_ptr(new Actor());
	result->_Fighter->Skills.clear();
	Skill_ptr smack = Skill_ptr(new SkillSmack());
	Skill_ptr heal = Skill_ptr(new SkillHeal());
	result->_Fighter->Skills.push_back(smack);
	result->_Fighter->Skills.push_back(heal);

	result->_Fighter->SetLevel(4);
	result->_Fighter->SetStatsFromCurve();
	//result->_Fighter->Speed = 1;
	//result->_Fighter->Strength = 2;
	//result->_Fighter->Crit = 3;
	//result->_Fighter->Defense = 1;
	//result->_Fighter->SetEndurance(4);
	PassiveFactory::ApplyAllPassives(result->_Fighter, &result->_Fighter->_Passives);
	result->_Fighter->Health = result->_Fighter->GetMaxHealth().Real;
	result->Sprite = "res/sprites/entities/entity_girl.png";
	result->_Graphics->SetTexture(result->Sprite);

	return result;
}

Actor_ptr ActorFactory::BuildBaseEnemy()
{
	Actor_ptr result = Actor_ptr(new Actor());
	result->_Fighter->Speed = 2;
	result->_Fighter->Strength = 2;
	result->_Fighter->Crit = 1;
	result->_Fighter->Defense = 0;
	result->_Fighter->SetEndurance(2);
	PassiveFactory::ApplyAllPassives(result->_Fighter, &result->_Fighter->_Passives);
	result->_Fighter->Health = result->_Fighter->GetMaxHealth().Real;
	result->_Name = "Slime";
	result->_Fighter->Team = 1;
	result->_Graphics->SetTexture("res/sprites/entities/entity_slime.png");

	return result;
}

std::vector<Actor_ptr> ActorFactory::BuildParty(rapidjson::GenericArray<false, rapidjson::Value>& arr)
{
	std::vector<Actor_ptr> result;
	for (auto& a : arr)
	{
		Actor_ptr actor = Actor_ptr(new Actor());
		actor->_Fighter->Skills.clear();
		if (a.HasMember("health"))
			actor->_Fighter->Health = a["health"].GetInt();

		if (a.HasMember("max_health"))
			actor->_Fighter->SetMaxHealth(a["max_health"].GetInt());

		if (a.HasMember("name"))
			actor->_Name = a["name"].GetString();

		if (a.HasMember("curve"))
			actor->_Fighter->Curve = a["curve"].GetString();

		if (a.HasMember("level"))
			actor->_Fighter->SetLevel(a["level"].GetInt());

		if (a.HasMember("exp"))
			actor->_Fighter->SetExp(a["level"].GetInt());

		if (a.HasMember("skillpoints"))
			actor->_Fighter->SkillPoints = a["skillpoints"].GetInt();

		if (a.HasMember("speed"))
			actor->_Fighter->Speed = a["speed"].GetInt();

		if (a.HasMember("strength"))
			actor->_Fighter->Strength = a["strength"].GetInt();

		if (a.HasMember("endurance"))
			actor->_Fighter->SetEndurance(a["endurance"].GetInt());

		if (a.HasMember("defense"))
			actor->_Fighter->Defense = a["defense"].GetInt();

		if (a.HasMember("crit"))
			actor->_Fighter->Crit = a["crit"].GetInt();

		if (a.HasMember("dead"))
			actor->_Fighter->Dead = a["dead"].GetBool();

		if (a.HasMember("sprite"))
		{
			actor->Sprite = a["sprite"].GetString();
			actor->_Graphics->SetTexture(actor->Sprite);
		}

		if (a.HasMember("skills") && a["skills"].IsArray())
		{
			auto skills = a["skills"].GetArray();
			for (rapidjson::Value::ConstValueIterator itr = skills.Begin(); itr != skills.End(); ++itr)
				actor->_Fighter->Skills.push_back(BuildSkill(itr->GetString()));
		}

		PassiveFactory::ApplyAllPassives(actor->_Fighter, &actor->_Fighter->_Passives);
		result.push_back(actor);
	}

	return result;
}

Skill_ptr ActorFactory::BuildSkill(std::string name)
{
	Skill_ptr result;
	if (name == "Smack")
		result = Skill_ptr(new SkillSmack());
	else if (name == "Heal")
		result = Skill_ptr(new SkillHeal());
	else if (name == "Protect")
		result = Skill_ptr(new SkillProtect());

	return result;
}
