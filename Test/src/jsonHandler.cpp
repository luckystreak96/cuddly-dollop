#include "jsonHandler.h"

JsonHandler::JsonHandler()
{
	JsonHandler::DataDocument = rapidjson::Document();
	JsonHandler::ReferenceDocument = rapidjson::Document();
	JsonHandler::File = "";
}

bool JsonHandler::DocumentNotNull()
{
	return ReferenceDocument.IsObject();
}

rapidjson::Document& JsonHandler::LoadJsonFromFile(std::string filename)
{
	File = Utils::ReadFile(filename);
	//rapidjson::Document doc;

	ReferenceDocument.Parse(File.c_str());

	if (ReferenceDocument.IsObject())
	{
		return ReferenceDocument;
		//if (doc.HasMember("hello"))
		//	std::cout << doc["hello"].GetString() << std::endl;
	}
	else
	{
		std::cout << "The following JSON document is invalid: " << filename << std::endl;
		return ReferenceDocument;
	}
}

rapidjson::Value JsonHandler::LoadEntities(int map_id)
{
	auto& m = LoadMap(map_id);
	if (m.HasMember("entities") && m["entities"].IsArray())
		return m["entities"].GetArray();

	std::cout << "Entities doesn't exist in the map" << std::endl;

	return rapidjson::Value(rapidjson::Type::kArrayType);
}

rapidjson::Value JsonHandler::LoadQueues(int map_id, int entity_id)
{
	auto& a = LoadEntities(map_id).GetArray();
	// Go through the entities
	for (auto& ent : a)
	{
		// If the entity is you...
		if (ent["id"].GetInt() == entity_id)
		{
			// Check out all the queues
			if (ent.HasMember("queues"))
				if (ent["queues"].IsArray())
					return ent["queues"].GetArray();
		}
	}

	return rapidjson::Value(rapidjson::Type::kArrayType);
}

rapidjson::Value JsonHandler::LoadMaps()
{
	rapidjson::Document::AllocatorType& a = ReferenceDocument.GetAllocator();
	DataDocument.CopyFrom(ReferenceDocument, a);

	if (DataDocument.IsObject())
		// If the json file has entities and the entities is an array
		if (DataDocument.HasMember("maps") && DataDocument["maps"].IsArray())
			return DataDocument["maps"].GetArray();
		else if (DataDocument.HasMember("tiles"))
			if (DataDocument["tiles"].IsArray())
				return DataDocument.GetObject();

	std::cout << "Maps don't exist in the file" << std::endl;

	return rapidjson::Value();
}

rapidjson::Value JsonHandler::LoadMap(int map_id)
{
	//auto& temp = LoadMaps();
	//if (temp.IsNull())
	//	return;
	auto& ms = LoadMaps();

	if (ms.IsArray())
		// Go through the entities
		for (unsigned int i = 0; i < ms.Size(); i++)
		{
			// If the entity is you...
			if (ms[i]["id"] == map_id)
			{
				return ms[i].GetObject();
			}
		}
	else
		return ms.GetObject();

	std::cout << "Map doesn't exist in the file" << std::endl;

	return rapidjson::Value();
}


void JsonHandler::SetFile(std::string filename)
{
	LoadJsonFromFile(filename);
}
