#include "fighterFactory.h"
#include "skillSmack.h"
#include "skillHeal.h"
#include "skillProtect.h"
#include "skillBloodyMess.h"
#include "passiveFactory.h"

Fighter_ptr FighterFactory::BuildBaseAlly()
{
	Fighter_ptr result = Fighter_ptr(new Fighter());
	std::vector<Skill_ptr> skills;

	Skill_ptr smack = Skill_ptr(new SkillSmack());
	Skill_ptr heal = Skill_ptr(new SkillHeal());
	skills.push_back(smack);
	skills.push_back(heal);

	result->SetSkills(skills);

	result->SetLevel(1);
	result->SetStatsFromCurve();
	//result->_Fighter->Speed = 1;
	//result->_Fighter->Strength = 2;
	//result->_Fighter->Crit = 3;
	//result->_Fighter->Defense = 1;
	//result->_Fighter->SetEndurance(4);
	PassiveFactory::ApplyAllPassives(result.get(), &result->_Passives);
	result->Health = result->GetMaxHealth().Real;
	result->SetSprite("res/sprites/entities/entity_girl.png");

	result->ResetModified();

	return result;
}

Fighter_ptr FighterFactory::BuildBaseEnemy()
{
	Fighter_ptr result = Fighter_ptr(new Fighter());
	result->SetLevel(rand() % 3 + 1);
	result->Curve = "Attack_Level_1";
	result->SetStatsFromCurve();
	//result->_Fighter->Speed = 2;
	//result->_Fighter->Strength = 2;
	//result->_Fighter->Crit = 1;
	//result->_Fighter->Defense = 0;
	//result->_Fighter->SetEndurance(2);
	PassiveFactory::ApplyAllPassives(result.get(), &result->_Passives);
	result->Health = result->GetMaxHealth().Real;
	result->SetName("Slime");
	result->Team = 1;
	result->SetSprite("res/sprites/entities/entity_slime.png");

	return result;
}

Fighter_ptr FighterFactory::BuildFighter(int id, int team, int level, int fluctuation)
{
	Fighter_ptr result = Fighter_ptr(new Fighter(*BattleData::Fighters.at(id)));
	result->Team = team;
	result->SetLevel(level + rand() % fluctuation);
	result->Health = result->GetMaxHealth().Real;
	return result;
}
Fighter_ptr FighterFactory::BuildFighter(rapidjson::Value& a)
{
	Fighter_ptr fighter = Fighter_ptr(new Fighter());

	//fighter->_Fighter->m_skills.clear();
	if (a.HasMember("health"))
		fighter->Health = a["health"].GetInt();

	//if (a.HasMember("max_health"))
	//	fighter->_Fighter->SetMaxHealth(a["max_health"].GetInt());

	if (a.HasMember("name"))
		fighter->GetName() = a["name"].GetString();

	if (a.HasMember("curve"))
		fighter->Curve = a["curve"].GetString();

	if (a.HasMember("level"))
		fighter->SetLevel(a["level"].GetInt());

	if (a.HasMember("exp"))
		fighter->SetExp(a["exp"].GetInt());

	if (a.HasMember("skillpoints"))
		fighter->SkillPoints = a["skillpoints"].GetInt();

	//if (a.HasMember("speed"))
	//	fighter->_Fighter->Speed = a["speed"].GetInt();

	//if (a.HasMember("strength"))
	//	fighter->_Fighter->Strength = a["strength"].GetInt();

	//if (a.HasMember("endurance"))
	//	fighter->_Fighter->SetEndurance(a["endurance"].GetInt());

	//if (a.HasMember("defense"))
	//	fighter->_Fighter->Defense = a["defense"].GetInt();

	//if (a.HasMember("crit"))
	//	fighter->_Fighter->Crit = a["crit"].GetInt();

	if (a.HasMember("dead"))
		fighter->Dead = a["dead"].GetBool();

	if (a.HasMember("sprite"))
		fighter->SetSprite(a["sprite"].GetString());

	if (a.HasMember("skills") && a["skills"].IsArray())
	{
		auto skills = a["skills"].GetArray();

		std::vector<Skill_ptr> skill_list;

		for (rapidjson::Value::ConstValueIterator itr = skills.Begin(); itr != skills.End(); ++itr)
			skill_list.push_back(BuildSkill(itr->GetString()));
		fighter->SetSkills(skill_list);
	}

	if (a.HasMember("passives") && a["passives"].IsArray())
	{
		auto passive = a["passives"].GetArray();
		for (rapidjson::Value::ConstValueIterator itr = passive.Begin(); itr != passive.End(); ++itr)
			fighter->_Passives.push_back(BattleData::PassiveSkills.at(itr->GetInt()));
	}

	// Stats are decided first by the curve, then the passives, then the equipment etc
	fighter->SetStatsFromCurve();
	PassiveFactory::ApplyAllPassives(fighter.get(), &fighter->_Passives);
	fighter->CurrentHealthCheck();

	return fighter;
}

std::vector<Fighter_ptr> FighterFactory::BuildParty(rapidjson::GenericArray<false, rapidjson::Value>& arr)
{
	std::vector<Fighter_ptr> result;
	for (auto& a : arr)
	{
		//Actor_ptr fighter = Actor_ptr(new Actor());
		////fighter->_Fighter->m_skills.clear();
		//if (a.HasMember("health"))
		//	fighter->_Fighter->Health = a["health"].GetInt();

		////if (a.HasMember("max_health"))
		////	fighter->_Fighter->SetMaxHealth(a["max_health"].GetInt());

		//if (a.HasMember("name"))
		//	fighter->_Name = a["name"].GetString();

		//if (a.HasMember("curve"))
		//	fighter->_Fighter->Curve = a["curve"].GetString();

		//if (a.HasMember("level"))
		//	fighter->_Fighter->SetLevel(a["level"].GetInt());

		//if (a.HasMember("exp"))
		//	fighter->_Fighter->SetExp(a["exp"].GetInt());

		//if (a.HasMember("skillpoints"))
		//	fighter->_Fighter->SkillPoints = a["skillpoints"].GetInt();

		////if (a.HasMember("speed"))
		////	fighter->_Fighter->Speed = a["speed"].GetInt();

		////if (a.HasMember("strength"))
		////	fighter->_Fighter->Strength = a["strength"].GetInt();

		////if (a.HasMember("endurance"))
		////	fighter->_Fighter->SetEndurance(a["endurance"].GetInt());

		////if (a.HasMember("defense"))
		////	fighter->_Fighter->Defense = a["defense"].GetInt();

		////if (a.HasMember("crit"))
		////	fighter->_Fighter->Crit = a["crit"].GetInt();

		//if (a.HasMember("dead"))
		//	fighter->_Fighter->Dead = a["dead"].GetBool();

		//if (a.HasMember("sprite"))
		//{
		//	fighter->Sprite = a["sprite"].GetString();
		//	fighter->_Graphics->SetTexture(fighter->Sprite);
		//}

		//if (a.HasMember("skills") && a["skills"].IsArray())
		//{
		//	auto skills = a["skills"].GetArray();
		//	for (rapidjson::Value::ConstValueIterator itr = skills.Begin(); itr != skills.End(); ++itr)
		//		fighter->_Fighter->m_skills.push_back(BuildSkill(itr->GetString()));
		//}

		//if (a.HasMember("passives") && a["passives"].IsArray())
		//{
		//	auto passive = a["passives"].GetArray();
		//	for (rapidjson::Value::ConstValueIterator itr = passive.Begin(); itr != passive.End(); ++itr)
		//		fighter->_Fighter->_Passives.push_back(BattleData::PassiveSkills.at(itr->GetInt()));
		//}

		//// Stats are decided first by the curve, then the passives, then the equipment etc
		//fighter->_Fighter->SetStatsFromCurve();
		//PassiveFactory::ApplyAllPassives(fighter->_Fighter.get(), &fighter->_Fighter->_Passives);
		//fighter->_Fighter->CurrentHealthCheck();

		//result.push_back(fighter);
		result.push_back(BuildFighter(a));
	}

	return result;
}

Skill_ptr FighterFactory::BuildSkill(std::string name)
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
