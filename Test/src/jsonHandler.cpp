#include "jsonHandler.h"

rapidjson::Document JsonHandler::LoadJsonFromFile(std::string filename)
{
	std::string file = Utils::ReadFile(filename);
	rapidjson::Document doc;

	doc.Parse(file.c_str());

	if (doc.IsObject())
	{
		return doc;
		//if (doc.HasMember("hello"))
		//	std::cout << doc["hello"].GetString() << std::endl;
	}
	else
	{
		std::cout << "The following JSON document is invalid: " << filename << std::endl;
		return doc;
	}
}
