#include "entityFactory.h"

float EntityFactory::GetFloat(EntityArgType eat)
{
	if (std::holds_alternative<float>(eat))
		return std::get<float>(eat);
	if (std::holds_alternative<int>(eat))
		return (float)std::get<int>(eat);

	// If we get here then its not even an int so wtf -_-
	return std::get<float>(eat);
}

std::shared_ptr<Entity> EntityFactory::BuildEntity(std::map<std::string, EntityArgType> args)
{
	std::shared_ptr<Entity> result = NULL;

	bool isPlayer = args.count("player") ? std::get<bool>(args.at("player")) : false;

	std::string sprite = "res/sprites/entities/" + (args.count("sprite") ? std::get<std::string>(args.at("sprite")) : "ghost.png");

	// ID
	if (args.count("id"))
	{
		unsigned int id = std::get<unsigned int>(args.at("id"));
		result = std::shared_ptr<Entity>(new Entity(id, sprite, isPlayer));
	}
	else
		// Sketchy af
		result = std::shared_ptr<Entity>(new Entity((unsigned int)rand() + (unsigned int)(MAXINT / 2), sprite));

	// Position
	if (args.count("x"))
	{
		float x = GetFloat(args.at("x"));
		float y = GetFloat(args.at("y"));
		float z = GetFloat(args.at("z"));
		result->Physics()->AbsolutePosition(Vector3f(x, y, z));
	}
	else
		result->Physics()->AbsolutePosition(Vector3f(1.f, 1.f, 4.f));

	// Ethereal
	bool ethereal = false;
	if (args.count("ethereal"))
		ethereal = std::get<bool>(args.at("ethereal"));
	result->Physics()->SetEthereal(ethereal);

	return result;
}

std::map<unsigned int, std::shared_ptr<Entity>> EntityFactory::GetEntities(unsigned int map_id, std::shared_ptr<JsonHandler> jh)
{
	std::map<unsigned int, std::shared_ptr<Entity>> result = std::map<unsigned int, std::shared_ptr<Entity>>();

	auto& entities = jh->LoadEntities(map_id);
	if (entities.IsNull())
		return result;

	auto& ents = entities.GetArray();

	// Get the args for each entity
	for (auto& x : ents)
	{
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
				eat = std::string(iter->value.GetString());
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

		//Build the entity
		std::shared_ptr<Entity> temp = BuildEntity(args);
		result.emplace(temp->GetID(), temp);
	}

	return result;
}
