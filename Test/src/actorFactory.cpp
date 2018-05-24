#include "actorFactory.h"
#include "skillSmack.h"
#include "skillHeal.h"
#include "skillProtect.h"
#include "skillBloodyMess.h"
#include "passiveFactory.h"

Actor_ptr ActorFactory::BuildBaseAlly()
{
	Actor_ptr result = Actor_ptr(new Actor());
	result->_Fighter->Skills.clear();
	Skill_ptr smack = Skill_ptr(new SkillSmack());
	Skill_ptr heal = Skill_ptr(new SkillHeal());
	result->_Fighter->Skills.push_back(smack);
	result->_Fighter->Skills.push_back(heal);

	result->_Fighter->SetLevel(1);
	result->_Fighter->SetStatsFromCurve();
	//result->_Fighter->Speed = 1;
	//result->_Fighter->Strength = 2;
	//result->_Fighter->Crit = 3;
	//result->_Fighter->Defense = 1;
	//result->_Fighter->SetEndurance(4);
	PassiveFactory::ApplyAllPassives(result->_Fighter.get(), &result->_Fighter->_Passives);
	result->_Fighter->Health = result->_Fighter->GetMaxHealth().Real;
	result->Sprite = "res/sprites/entities/entity_girl.png";
	result->_Graphics->SetTexture(result->Sprite);

	result->_Fighter->ResetModified();

	return result;
}

Actor_ptr ActorFactory::BuildBaseEnemy()
{
	Actor_ptr result = Actor_ptr(new Actor());
	result->_Fighter->SetLevel(rand() % 3 + 1);
	result->_Fighter->Curve = "Attack_Level_1";
	result->_Fighter->SetStatsFromCurve();
	//result->_Fighter->Speed = 2;
	//result->_Fighter->Strength = 2;
	//result->_Fighter->Crit = 1;
	//result->_Fighter->Defense = 0;
	//result->_Fighter->SetEndurance(2);
	PassiveFactory::ApplyAllPassives(result->_Fighter.get(), &result->_Fighter->_Passives);
	result->_Fighter->Health = result->_Fighter->GetMaxHealth().Real;
	result->_Name = "Slime";
	result->_Fighter->Team = 1;
	result->_Graphics->SetTexture("res/sprites/entities/entity_slime.png");

	return result;
}

Actor_ptr ActorFactory::BuildActor(int id, int team, int level, int fluctuation)
{
	Actor_ptr actor = Actor_ptr(new Actor(*BattleData::Actors.at(id)));
	actor->_Fighter->Team = team;
	actor->_Fighter->SetLevel(level + rand() % fluctuation);
	actor->_Fighter->Health = actor->_Fighter->GetMaxHealth().Real;
	return actor;
}
Actor_ptr ActorFactory::BuildActor(rapidjson::Value& a)
{
	Actor_ptr actor = Actor_ptr(new Actor());

	actor->_Fighter->Skills.clear();
	if (a.HasMember("health"))
		actor->_Fighter->Health = a["health"].GetInt();

	//if (a.HasMember("max_health"))
	//	actor->_Fighter->SetMaxHealth(a["max_health"].GetInt());

	if (a.HasMember("name"))
		actor->_Name = a["name"].GetString();

	if (a.HasMember("curve"))
		actor->_Fighter->Curve = a["curve"].GetString();

	if (a.HasMember("level"))
		actor->_Fighter->SetLevel(a["level"].GetInt());

	if (a.HasMember("exp"))
		actor->_Fighter->SetExp(a["exp"].GetInt());

	if (a.HasMember("skillpoints"))
		actor->_Fighter->SkillPoints = a["skillpoints"].GetInt();

	//if (a.HasMember("speed"))
	//	actor->_Fighter->Speed = a["speed"].GetInt();

	//if (a.HasMember("strength"))
	//	actor->_Fighter->Strength = a["strength"].GetInt();

	//if (a.HasMember("endurance"))
	//	actor->_Fighter->SetEndurance(a["endurance"].GetInt());

	//if (a.HasMember("defense"))
	//	actor->_Fighter->Defense = a["defense"].GetInt();

	//if (a.HasMember("crit"))
	//	actor->_Fighter->Crit = a["crit"].GetInt();

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

	if (a.HasMember("passives") && a["passives"].IsArray())
	{
		auto passive = a["passives"].GetArray();
		for (rapidjson::Value::ConstValueIterator itr = passive.Begin(); itr != passive.End(); ++itr)
			actor->_Fighter->_Passives.push_back(BattleData::PassiveSkills.at(itr->GetInt()));
	}

	// Stats are decided first by the curve, then the passives, then the equipment etc
	actor->_Fighter->SetStatsFromCurve();
	PassiveFactory::ApplyAllPassives(actor->_Fighter.get(), &actor->_Fighter->_Passives);
	actor->_Fighter->CurrentHealthCheck();

	return actor;
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

		//if (a.HasMember("max_health"))
		//	actor->_Fighter->SetMaxHealth(a["max_health"].GetInt());

		if (a.HasMember("name"))
			actor->_Name = a["name"].GetString();

		if (a.HasMember("curve"))
			actor->_Fighter->Curve = a["curve"].GetString();

		if (a.HasMember("level"))
			actor->_Fighter->SetLevel(a["level"].GetInt());

		if (a.HasMember("exp"))
			actor->_Fighter->SetExp(a["exp"].GetInt());

		if (a.HasMember("skillpoints"))
			actor->_Fighter->SkillPoints = a["skillpoints"].GetInt();

		//if (a.HasMember("speed"))
		//	actor->_Fighter->Speed = a["speed"].GetInt();

		//if (a.HasMember("strength"))
		//	actor->_Fighter->Strength = a["strength"].GetInt();

		//if (a.HasMember("endurance"))
		//	actor->_Fighter->SetEndurance(a["endurance"].GetInt());

		//if (a.HasMember("defense"))
		//	actor->_Fighter->Defense = a["defense"].GetInt();

		//if (a.HasMember("crit"))
		//	actor->_Fighter->Crit = a["crit"].GetInt();

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

		if (a.HasMember("passives") && a["passives"].IsArray())
		{
			auto passive = a["passives"].GetArray();
			for (rapidjson::Value::ConstValueIterator itr = passive.Begin(); itr != passive.End(); ++itr)
				actor->_Fighter->_Passives.push_back(BattleData::PassiveSkills.at(itr->GetInt()));
		}

		// Stats are decided first by the curve, then the passives, then the equipment etc
		actor->_Fighter->SetStatsFromCurve();
		PassiveFactory::ApplyAllPassives(actor->_Fighter.get(), &actor->_Fighter->_Passives);
		actor->_Fighter->CurrentHealthCheck();

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
	else if (name == "Bloody Mess")
		result = Skill_ptr(new SkillBloodyMess());

	return result;
}
