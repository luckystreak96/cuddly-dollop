#include "battleData.h"

#include <iostream>

#include "fighterFactory.h"
#include "statCurve.h"
#include "passiveFactory.h"

#undef GetObject

using namespace rapidjson;

std::vector<Fighter_ptr> BattleData::Party = std::vector<Fighter_ptr>();
std::map<int, Passive_ptr> BattleData::PassiveSkills = std::map<int, Passive_ptr>();
std::map<int, Fighter_ptr> BattleData::Fighters = std::map<int, Fighter_ptr>();

std::map<Status, StatusEffect> BattleData::StatusEffects = std::map<Status, StatusEffect>();
	

void BattleData::LoadStatusEffects()
{
	// DETERMINED
	Status status = Status_Determined;
	StatusEffect result;
	result._Name = "Determined";
	result._Element = SE_Determined;

	auto passive = PassiveFactory::BuildPassive("", PassivePriority::PP_AbsoluteFirst, PassiveType::PT_Stat, PassiveSpecifier::PS_Flat, 5, 0, "strength");
	result._Effects.push_back(std::tuple<Passive_ptr, int>(passive, 1));
	passive = PassiveFactory::BuildPassive("", PassivePriority::PP_AbsoluteFirst, PassiveType::PT_Stat, PassiveSpecifier::PS_Percent, 0.1, 0, "strength");
	result._Effects.push_back(std::tuple<Passive_ptr, int>(passive, 1));

	StatusEffects.emplace(status, result);

	// PRAGMATIC
	status = Status_Pragmatic;
	result._Name = "Pragmatic";
	result._Element = SE_Pragmatic;

	passive = PassiveFactory::BuildPassive("", PassivePriority::PP_AbsoluteFirst, PassiveType::PT_Stat, PassiveSpecifier::PS_Flat, 1, 1, "max_health");
	result._Effects.push_back(std::tuple<Passive_ptr, int>(passive, 1));

	StatusEffects.emplace(status, result);

	// PROTECTED
	status = Status_Protected;
	result._Name = "Protected";
	result._Element = SE_Pragmatic;
	result._Conditions.push_back(TC_Targeted);
	result._Conditions.push_back(TC_Applier_Alive);

	passive = PassiveFactory::BuildPassive("Protected", PassivePriority::PP_BeforeSkill, PassiveType::PT_Special, PassiveSpecifier::PS_Flat, 0, 0, "owner");
	result._Effects.push_back(std::tuple<Passive_ptr, int>(passive, 1));
	result._Actions.push_back(std::pair<SkillProgress, triple>(SP_1_Before_Anim, triple(AS_JumpTo, AARG_Passive_Applier2Owner, floats({})))); // jump to target
	result._Actions.push_back(std::pair<SkillProgress, triple>(SP_1_Before_Anim, triple(AA_ChangeTarget, AARG_Passive_Applier2Owner, floats({})))); // jump to target
	result._Actions.push_back(std::pair<SkillProgress, triple>(SP_4_PostSkillAnim, triple(AS_JumpBack, AARG_Passive_Applier2Owner, floats({})))); // jump back

	StatusEffects.emplace(status, result);
}

void BattleData::NewGame()
{
	if (!Party.size() > 0)
		Party.push_back(FighterFactory::BuildBaseAlly());
}

void BattleData::LoadAll()
{
	LoadStatusEffects();
	LoadCurves();
	LoadPassives();
	LoadActors();
}

void BattleData::LoadCurves()
{
	std::string file = "";
	try {
		file = Utils::ReadFile("res/data/battle/battle.json");
	}
	catch (...)// catch any exception
	{
		std::cout << "Error loading: res/data/battle/battle.json" << std::endl;
		return;
	}

	rapidjson::Document doc;
	doc.Parse(file.c_str());

	// Get the flags
	if (doc.HasMember("curves") && doc["curves"].IsObject())
	{
		std::map<std::string, CurveInfo> data;
		auto curves = doc["curves"].GetObject();
		// Loop through curves
		for (Value::ConstMemberIterator itr = curves.MemberBegin(); itr != curves.MemberEnd(); ++itr)
		{
			std::string curveName = itr->name.GetString();
			CurveInfo stats;

			auto stat = itr->value.GetObject();
			// Loop through the stats of a curve
			for (Value::ConstMemberIterator itr2 = stat.MemberBegin(); itr2 != stat.MemberEnd(); ++itr2)
			{
				std::string statName = itr2->name.GetString();
				CurveStyle style;
				// Get the op and value
				for (auto& statInfo : itr2->value.GetObject())
				{
					// Get the name
					std::string statDetailIdent = statInfo.name.GetString();

					// Get the value, look out for some int bullshit
					float statValue;
					std::string statValueString;
					if (statInfo.value.IsInt())
						statValue = (float)statInfo.value.GetInt();
					else if (statInfo.value.IsFloat())
						statValue = statInfo.value.GetFloat();
					else
						statValueString = statInfo.value.GetString();

					if (statDetailIdent == "op")
						style.StatOperator = statValueString;
					else if (statDetailIdent == "value")
						style.Value = statValue;
				}

				stats.Stats.emplace(statName, style);
			}

			// emplace curve
			data.emplace(curveName, stats);
		}

		StatCurve::Curves = data;
	}
}

void BattleData::LoadPassives()
{
	std::string file = "";
	try {
		file = Utils::ReadFile("res/data/battle/battle.json");
	}
	catch (...)// catch any exception
	{
		std::cout << "Error loading: res/data/battle/battle.json" << std::endl;
		return;
	}

	rapidjson::Document doc;
	doc.Parse(file.c_str());

	// Get the flags
	if (doc.HasMember("passives") && doc["passives"].IsArray())
	{
		std::map<int, Passive_ptr> data;
		auto passives = doc["passives"].GetArray();
		// Loop through curves
		for (auto& passive : passives)
		{
			int passiveID = -1;
			if (passive.HasMember("id"))
				passiveID = passive["id"].GetInt();

			std::string passiveName = "";
			if (passive.HasMember("name"))
				passiveName = passive["name"].GetString();

			PassivePriority priority = PP_AbsoluteFirst;
			if (passive.HasMember("priority"))
				priority = (PassivePriority)PassiveSkill::EnumToInt[passive["priority"].GetString()];

			PassiveType type = PT_Stat;
			if (passive.HasMember("type"))
				type = (PassiveType)PassiveSkill::EnumToInt[passive["type"].GetString()];

			PassiveSpecifier specifier = PS_Flat;
			if (passive.HasMember("specifier"))
				specifier = (PassiveSpecifier)PassiveSkill::EnumToInt[passive["specifier"].GetString()];

			PassiveData ifs;

			if (passive.HasMember("_int"))
				ifs._Integer = passive["_int"].GetInt();

			if (passive.HasMember("_float"))
				ifs._Float = passive["_float"].GetFloat();

			if (passive.HasMember("_string"))
				ifs._String = passive["_string"].GetString();

			Passive_ptr skill = Passive_ptr(new PassiveSkill());
			skill->_Data = ifs;
			skill->_Id = passiveID;
			skill->_Name = passiveName;
			skill->_Priority = priority;
			skill->_Specifier = specifier;
			skill->_Type = type;

			// emplace curve
			data.emplace(passiveID, skill);
		}

		PassiveSkills = data;
	}
}

void BattleData::LoadActors()
{
	std::string file = "";
	try {
		file = Utils::ReadFile("res/data/battle/battle.json");
	}
	catch (...)// catch any exception
	{
		std::cout << "Error loading: res/data/battle/battle.json" << std::endl;
		return;
	}

	rapidjson::Document doc;
	doc.Parse(file.c_str());

	// Get the flags
	if (doc.HasMember("fighters") && doc["fighters"].IsArray())
	{
		std::map<int, Fighter_ptr> data;
		auto fighters = doc["fighters"].GetArray();
		// Loop through actors
		for (auto& fighter : fighters)
		{
			int fighterID = -1;
			if (fighter.HasMember("id"))
				fighterID = fighter["id"].GetInt();

			// emplace fighter
			data.emplace(fighterID, FighterFactory::BuildFighter(fighter));
		}

		Fighters = data;
	}
}

void BattleData::LoadParty(rapidjson::Document& doc)
{
	if (doc.HasMember("party") && doc["party"].IsArray())
	{
		auto party = doc["party"].GetArray();
		BattleData::Party = FighterFactory::BuildParty(party);
	}
}

void BattleData::SaveParty(rapidjson::Document& saveFile, Document::AllocatorType& allocator)
{
	// Save party data
	Value party_v(kArrayType);
	for (auto& fighter : Party)
	{
		Value ob(kObjectType);
		Value first(kStringType);
		Value second;

		// name
		first.SetString(StringRef("name"), allocator);
		second.SetString(StringRef(fighter->GetName().c_str()), allocator);
		ob.AddMember(first, second, allocator);

		// sprite
		first.SetString(StringRef("sprite"), allocator);
		second.SetString(StringRef(fighter->GetSprite().c_str()), allocator);
		ob.AddMember(first, second, allocator);

		// curve
		first.SetString(StringRef("curve"), allocator);
		second.SetString(StringRef(fighter->Curve.c_str()), allocator);
		ob.AddMember(first, second, allocator);

		// health
		first.SetString(StringRef("health"), allocator);
		second.SetInt(fighter->Health);
		ob.AddMember(first, second, allocator);

		// maxHealth
		first.SetString(StringRef("max_health"), allocator);
		second.SetInt(fighter->GetMaxHealth().Base);
		ob.AddMember(first, second, allocator);

		// level
		first.SetString(StringRef("level"), allocator);
		second.SetInt(fighter->GetLevel());
		ob.AddMember(first, second, allocator);

		// exp
		first.SetString(StringRef("exp"), allocator);
		second.SetInt(fighter->GetExp());
		ob.AddMember(first, second, allocator);

		// skillpoints
		first.SetString(StringRef("skillpoints"), allocator);
		second.SetInt(fighter->SkillPoints);
		ob.AddMember(first, second, allocator);

		// speed
		first.SetString(StringRef("speed"), allocator);
		second.SetInt(fighter->Speed.Base);
		ob.AddMember(first, second, allocator);

		// crit
		first.SetString(StringRef("crit"), allocator);
		second.SetInt(fighter->Crit.Base);
		ob.AddMember(first, second, allocator);

		// defense 
		first.SetString(StringRef("defense"), allocator);
		second.SetInt(fighter->Defense.Base);
		ob.AddMember(first, second, allocator);

		// strength
		first.SetString(StringRef("strength"), allocator);
		second.SetInt(fighter->Strength.Base);
		ob.AddMember(first, second, allocator);

		// endurance
		first.SetString(StringRef("endurance"), allocator);
		second.SetInt(fighter->GetEndurance().Base);
		ob.AddMember(first, second, allocator);

		// dead
		first.SetString(StringRef("dead"), allocator);
		second.SetBool(fighter->Dead);
		ob.AddMember(first, second, allocator);

		// skills
		first.SetString(StringRef("skills"), allocator);
		second = Value(kArrayType);

		for (auto& x : fighter->GetSkills())
		{
			Value skl(kStringType);
			skl.SetString(StringRef(x->GetName().c_str()), allocator);
			second.PushBack(skl, allocator);
		}
		ob.AddMember(first, second, allocator);

		// passives
		first.SetString(StringRef("fighters"), allocator);
		second = Value(kArrayType);
		for (auto passive : fighter->_Passives)
		{
			Value skl(rapidjson::Type::kNumberType);
			skl.SetInt(passive->_Id);
			second.PushBack(skl, allocator);
		}
		ob.AddMember(first, second, allocator);

		party_v.PushBack(ob, allocator);
	}

	saveFile.AddMember("party", party_v, allocator);

}
