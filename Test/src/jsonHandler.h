#ifndef JSON_HANDLER_H__
#define JSON_HANDLER_H__

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <iostream>

#ifdef _WIN32
	#include <Windows.h>
#endif
#include <rapidjson/document.h>
#include "utils.h"

class JsonHandler
{
public:
	JsonHandler(int map_id);
	void LoadJsonFromFile();
	rapidjson::Value LoadEntities(int map_id);
	rapidjson::Value LoadQueues(int map_id, int entity_id);
	rapidjson::Value LoadMap(int map_id);
	rapidjson::Value LoadMaps(int map_id);
	void SetFile(std::string filename);
	bool DocumentNotNull();
public:
	static std::map<int, rapidjson::Document> ReferenceDocument;
	static bool FilesLoaded;
	int m_mapId;
	rapidjson::Document DataDocument;
private:
	std::string File;
};

#endif // !JSON_HANDLER_H__
