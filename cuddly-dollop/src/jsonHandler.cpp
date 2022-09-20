#include "jsonHandler.h"
#include <filesystem>

std::map<int, rapidjson::Document> JsonHandler::ReferenceDocument;
bool JsonHandler::FilesLoaded = false;


JsonHandler::JsonHandler(int map_id) : m_mapId(map_id)
{
	LoadJsonFromFile();

	rapidjson::Document::AllocatorType& a = ReferenceDocument.at(map_id).GetAllocator();
	DataDocument.CopyFrom(ReferenceDocument.at(map_id), a);
	//JsonHandler::ReferenceDocument = rapidjson::Document();
	JsonHandler::File = "";
}

bool JsonHandler::DocumentNotNull()
{
	return false;
	//return ReferenceDocument.IsObject();
}

void JsonHandler::LoadJsonFromFile()
{
	if (FilesLoaded)
		return;

	std::vector<std::string> vs = Utils::GetAllFiles("res/data/", "json");
	/*
#ifdef _WIN32
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	hFind = FindFirstFile(L"res/data/*.json", &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			std::wstring w = FindFileData.cFileName;
			vs.push_back(std::string(w.begin(), w.end()));
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
#else
	std::string path = "res/data/";
	for (auto & p : std::experimental::filesystem::directory_iterator(path))
		std::cout << p << std::endl;
#endif
*/

	for (auto f : vs)
	{
		File = Utils::ReadFile("res/data/" + f);
		int id = std::stoi(Utils::Split(f, '.').at(0));
		ReferenceDocument.emplace(id, rapidjson::Document());
		ReferenceDocument.at(id).Parse(File.c_str());

		//Make sure its kk
		if (!ReferenceDocument.at(id).IsObject())
		{
			std::cout << "The following JSON document is invalid: " << f << std::endl;
		}
	}

	FilesLoaded = true;
}

rapidjson::Value JsonHandler::LoadEntities(int map_id)
{
	auto m = LoadMap(map_id);
	if (m.HasMember("entities") && m["entities"].IsArray())
		return m["entities"].GetArray();

	std::cout << "Entities doesn't exist in the map" << std::endl;

	return rapidjson::Value(rapidjson::Type::kArrayType);
}

rapidjson::Value JsonHandler::LoadQueues(int map_id, int entity_id)
{
	auto a = LoadEntities(map_id).GetArray();
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

rapidjson::Value JsonHandler::LoadMaps(int map_id)
{
	rapidjson::Document::AllocatorType& a = ReferenceDocument.at(map_id).GetAllocator();
	DataDocument.CopyFrom(ReferenceDocument.at(map_id), a);

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
	auto ms = LoadMaps(map_id);

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
	//LoadJsonFromFile(filename);
}

