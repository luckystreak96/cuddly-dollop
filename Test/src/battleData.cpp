#include "battleData.h"
#include "actorFactory.h"

using namespace rapidjson;

std::vector<Actor_ptr> BattleData::Party = std::vector<Actor_ptr>();

void BattleData::NewGame()
{
	if (!Party.size() > 0)
		Party.push_back(ActorFactory::BuildBaseAlly());
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

		// health
		first.SetString(StringRef("health"), allocator);
		second.SetInt(actor->_Fighter->Health);
		ob.AddMember(first, second, allocator);

		// maxHealth
		first.SetString(StringRef("max_health"), allocator);
		second.SetInt(actor->_Fighter->GetMaxHealth());
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
		second.SetInt(actor->_Fighter->Speed);
		ob.AddMember(first, second, allocator);

		// crit
		first.SetString(StringRef("crit"), allocator);
		second.SetInt(actor->_Fighter->Crit);
		ob.AddMember(first, second, allocator);

		// defense 
		first.SetString(StringRef("defense"), allocator);
		second.SetInt(actor->_Fighter->Defense);
		ob.AddMember(first, second, allocator);

		// strength
		first.SetString(StringRef("strength"), allocator);
		second.SetInt(actor->_Fighter->Strength);
		ob.AddMember(first, second, allocator);

		// endurance
		first.SetString(StringRef("endurance"), allocator);
		second.SetInt(actor->_Fighter->GetEndurance());
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
