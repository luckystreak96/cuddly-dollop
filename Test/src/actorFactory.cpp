#include "actorFactory.h"
#include "skillSmack.h"
#include "skillHeal.h"

Actor_ptr ActorFactory::BuildBaseAlly()
{
	Actor_ptr result = Actor_ptr(new Actor());
	result->Speed = 1;

	return result;
}

Actor_ptr ActorFactory::BuildBaseEnemy()
{
	Actor_ptr result = Actor_ptr(new Actor());
	result->Speed = 2;
	result->Name = "Ghost";
	result->Team = 1;
	result->SetTexture("res/sprites/entities/entity_ghost.png");

	return result;
}

std::vector<Actor_ptr> ActorFactory::BuildParty(rapidjson::GenericArray<false, rapidjson::Value>& arr)
{
	std::vector<Actor_ptr> result;
	for (auto& a : arr)
	{
		Actor_ptr actor = Actor_ptr(new Actor());
		actor->Skills.clear();
		if (a.HasMember("health"))
			actor->Health = a["health"].GetInt();

		if (a.HasMember("max_health"))
			actor->MaxHealth = a["max_health"].GetInt();

		if (a.HasMember("name"))
			actor->Name = a["name"].GetString();

		if (a.HasMember("speed"))
			actor->Speed = a["speed"].GetInt();

		if (a.HasMember("dead"))
			actor->Dead = a["dead"].GetBool();

		if (a.HasMember("skills") && a["skills"].IsArray())
		{
			auto& skills = a["skills"].GetArray();
			for (rapidjson::Value::ConstValueIterator itr = skills.Begin(); itr != skills.End(); ++itr)
				actor->Skills.push_back(BuildSkill(itr->GetString()));
		}

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
	else
		result = Skill_ptr(new Skill());

	return result;
}