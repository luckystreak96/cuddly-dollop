#ifndef FIGHTER_FACTORY_H__
#define FIGHTER_FACTORY_H__

#include <string>
#include <vector>
#include <variant>
#include <map>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include "utils.h"
#include "fighter.h"
#include "skill.h"

class FighterFactory
{
	friend class Fighter;
public:
	static Fighter_ptr BuildBaseAlly();
	static Fighter_ptr BuildBaseEnemy();
	static std::vector<Fighter_ptr> BuildParty(rapidjson::GenericArray<false, rapidjson::Value>& arr);
	static Fighter_ptr BuildFighter(rapidjson::Value& a);
	static Fighter_ptr BuildFighter(int id, int team = 1, int level = 1, int fluctuation = 3);
	static Skill_ptr BuildSkill(std::string name);
};

#endif // !EVENT_FACTORY_H__

