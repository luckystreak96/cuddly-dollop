#include "battleData.h"

#include <iostream>

#include "actorFactory.h"
#include "statCurve.h"

#undef GetObject

using namespace rapidjson;

std::vector<Actor_ptr> BattleData::Party = std::vector<Actor_ptr>();

void BattleData::NewGame()
{
	if (!Party.size() > 0)
		Party.push_back(ActorFactory::BuildBaseAlly());
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
		auto curves = doc["curves"].GetObjectW();
		// Loop through curves
		for (Value::ConstMemberIterator itr = curves.MemberBegin(); itr != curves.MemberEnd(); ++itr)
		{
			std::string curveName = itr->name.GetString();
			CurveInfo stats;

			auto stat = itr->value.GetObjectW();
			// Loop through the stats of a curve
			for (Value::ConstMemberIterator itr2 = stat.MemberBegin(); itr2 != stat.MemberEnd(); ++itr2)
			{
				std::string statName = itr2->name.GetString();
				CurveStyle style;
				// Get the op and value
				for (auto& statInfo : itr2->value.GetObjectW())
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

void BattleData::LoadParty(rapidjson::Document& doc)
{
	if (doc.HasMember("party") && doc["party"].IsArray())
	{
		auto party = doc["party"].GetArray();
		BattleData::Party = ActorFactory::BuildParty(party);
	}
}

void BattleData::SaveParty(rapidjson::Document& saveFile, Document::AllocatorType& allocator)
{
	// Save party data
	Value party_v(kArrayType);
	for (auto actor : Party)
	{
		Value ob(kObjectType);
		Value first(kStringType);
		Value second;

		// name
		first.SetString(StringRef("name"), allocator);
		second.SetString(StringRef(actor->_Name.c_str()), allocator);
		ob.AddMember(first, second, allocator);

		// sprite
		first.SetString(StringRef("sprite"), allocator);
		second.SetString(StringRef(actor->Sprite.c_str()), allocator);
		ob.AddMember(first, second, allocator);

		// curve
		first.SetString(StringRef("curve"), allocator);
		second.SetString(StringRef(actor->_Fighter->Curve.c_str()), allocator);
		ob.AddMember(first, second, allocator);

		// health
		first.SetString(StringRef("health"), allocator);
		second.SetInt(actor->_Fighter->Health);
		ob.AddMember(first, second, allocator);

		// maxHealth
		first.SetString(StringRef("max_health"), allocator);
		second.SetInt(actor->_Fighter->GetMaxHealth().Base);
		ob.AddMember(first, second, allocator);

		// level
		first.SetString(StringRef("level"), allocator);
		second.SetInt(actor->_Fighter->GetLevel());
		ob.AddMember(first, second, allocator);

		// exp
		first.SetString(StringRef("exp"), allocator);
		second.SetInt(actor->_Fighter->GetExp());
		ob.AddMember(first, second, allocator);

		// skillpoints
		first.SetString(StringRef("skillpoints"), allocator);
		second.SetInt(actor->_Fighter->SkillPoints);
		ob.AddMember(first, second, allocator);

		// speed
		first.SetString(StringRef("speed"), allocator);
		second.SetInt(actor->_Fighter->Speed.Base);
		ob.AddMember(first, second, allocator);

		// crit
		first.SetString(StringRef("crit"), allocator);
		second.SetInt(actor->_Fighter->Crit.Base);
		ob.AddMember(first, second, allocator);

		// defense 
		first.SetString(StringRef("defense"), allocator);
		second.SetInt(actor->_Fighter->Defense.Base);
		ob.AddMember(first, second, allocator);

		// strength
		first.SetString(StringRef("strength"), allocator);
		second.SetInt(actor->_Fighter->Strength.Base);
		ob.AddMember(first, second, allocator);

		// endurance
		first.SetString(StringRef("endurance"), allocator);
		second.SetInt(actor->_Fighter->GetEndurance().Base);
		ob.AddMember(first, second, allocator);

		// dead
		first.SetString(StringRef("dead"), allocator);
		second.SetBool(actor->_Fighter->Dead);
		ob.AddMember(first, second, allocator);

		// skills
		first.SetString(StringRef("skills"), allocator);
		second = Value(kArrayType);
		for (auto skill : actor->_Fighter->Skills)
		{
			Value skl(kStringType);
			skl.SetString(StringRef(skill->_name.c_str()), allocator);
			second.PushBack(skl, allocator);
		}
		ob.AddMember(first, second, allocator);

		party_v.PushBack(ob, allocator);
	}

	saveFile.AddMember("party", party_v, allocator);

}
