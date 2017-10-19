#include "entityFactory.h"

Entity* EntityFactory::BuildEntity(std::map<std::string, EntityArgType> args)
{
	Entity* result = NULL;

	bool isPlayer = args.count("player");

	// ID
	if (args.count("id"))
	{
		unsigned int id = std::get<unsigned int>(args.at("id"));
		result = new Entity(id, isPlayer);
	}
	else
		// Sketchy af
		result = new Entity((unsigned int)rand() + (unsigned int)(MAXINT / 2));

	// Position
	if (args.count("position.x"))
	{
		float x = std::get<float>(args.at("position.x"));
		float y = std::get<float>(args.at("position.y"));
		float z = std::get<float>(args.at("position.z"));
		result->Physics()->AbsolutePosition(Vector3f(x, y, z));
	}
	else
		result->Physics()->AbsolutePosition(Vector3f(1.f, 1.f, 4.f));

	return result;
}

std::map<unsigned int, Entity*> EntityFactory::GetEntities(unsigned int map_id)
{
	std::map<unsigned int, Entity*> result = std::map<unsigned int, Entity*>();

	auto& ents = JsonHandler::LoadEntities(map_id).GetArray();


	for (auto& x : ents)
	{
		//unsigned int id = x["id"].GetUint();

		//Entity* temp = new Entity(id, isPlayer);
		std::map<std::string, EntityArgType> args = std::map<std::string, EntityArgType>();

		for (rapidjson::Value::ConstMemberIterator iter = x.MemberBegin(); iter != x.MemberEnd(); ++iter)
		{
			EntityArgType eat;
			rapidjson::Type t;
			t = iter->value.GetType();
			switch (t)
			{
			case rapidjson::Type::kNumberType:
				if (iter->value.IsFloat())
					eat = iter->value.GetFloat();
				if (iter->value.IsInt())
					eat = iter->value.GetInt();
				if (iter->value.IsUint())
					eat = iter->value.GetUint();
				break;
			case rapidjson::Type::kStringType:
				eat = iter->value.GetString();
				break;
			case rapidjson::Type::kTrueType:
				eat = iter->value.GetBool();
				break;
			case rapidjson::Type::kFalseType:
				eat = iter->value.GetBool();
				break;
			}

			args.emplace(std::string(iter->name.GetString()), eat);
		}

		Entity* temp = BuildEntity(args);
		result.emplace(temp->GetID(), temp);
	}

	return result;
}
