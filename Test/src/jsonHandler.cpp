#include "jsonHandler.h"

rapidjson::Document JsonHandler::DataDocument = rapidjson::Document();

rapidjson::Document& JsonHandler::LoadJsonFromFile(std::string filename)
{
	std::string file = Utils::ReadFile(filename);
	//rapidjson::Document doc;

	DataDocument.Parse(file.c_str());

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

rapidjson::Value JsonHandler::LoadEntities()
{
	if (DataDocument.IsObject())
		// If the json file has entities and the entities is an array
		if (DataDocument.HasMember("entities") && DataDocument["entities"].IsArray())
			return DataDocument["entities"].GetArray();

	std::cout << "Entities doesn't exist in the file :P" << std::endl;

	return rapidjson::Value();
}

rapidjson::Value JsonHandler::LoadQueues(int entity_id)
{
	// Go through the entities
	for (unsigned int i = 0; i < DataDocument["entities"].Size(); i++)
	{
		// If the entity is you...
		if (DataDocument["entities"][i]["id"] == entity_id)
		{
			// Check out all the queues
			//const auto& ques = arr[i]["queues"].GetArray();
			return DataDocument["entities"][i]["queues"].GetArray();
		}
	}

	return rapidjson::Value();
}