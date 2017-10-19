#include "jsonHandler.h"

rapidjson::Document JsonHandler::DataDocument = rapidjson::Document();
std::string JsonHandler::File = "";

rapidjson::Document& JsonHandler::LoadJsonFromFile(std::string filename)
{
	File = Utils::ReadFile(filename);
	//rapidjson::Document doc;

	DataDocument.Parse(File.c_str());

	if (DataDocument.IsObject())
	{
		return DataDocument;
		//if (doc.HasMember("hello"))
		//	std::cout << doc["hello"].GetString() << std::endl;
	}
	else
	{
		std::cout << "The following JSON document is invalid: " << filename << std::endl;
		return DataDocument;
	}
}

rapidjson::Value JsonHandler::LoadEntities(int map_id)
{
	auto& m = LoadMap(map_id);
		if (m.HasMember("entities") && m["entities"].IsArray())
			return m["entities"].GetArray();

	std::cout << "Entities doesn't exist in the map" << std::endl;

	return rapidjson::Value();
}

rapidjson::Value JsonHandler::LoadQueues(int map_id, int entity_id)
{
	auto& a = LoadEntities(map_id).GetArray();
	// Go through the entities
	for (unsigned int i = 0; i < a.Size(); i++)
	{
		// If the entity is you...
		if (a[i]["id"] == entity_id)
		{
			// Check out all the queues
			//const auto& ques = arr[i]["queues"].GetArray();
			return a[i]["queues"].GetArray();
		}
	}

	return rapidjson::Value();
}

rapidjson::Value JsonHandler::LoadMaps()
{
	DataDocument.Parse(File.c_str());

	if (DataDocument.IsObject())
		// If the json file has entities and the entities is an array
		if (DataDocument.HasMember("maps") && DataDocument["maps"].IsArray())
			return DataDocument["maps"].GetArray();

	std::cout << "Maps don't exist in the file" << std::endl;

	return rapidjson::Value();
}

rapidjson::Value JsonHandler::LoadMap(int map_id)
{
	auto& ms = LoadMaps().GetArray();
	// Go through the entities
	for (unsigned int i = 0; i < ms.Size(); i++)
	{
		// If the entity is you...
		if (ms[i]["id"] == map_id)
		{
			return ms[i].GetObject();
		}
	}

	return rapidjson::Value();

	std::cout << "Map doesn't exist in the file" << std::endl;

	return rapidjson::Value();
}