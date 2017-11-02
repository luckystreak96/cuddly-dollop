#ifndef JSON_HANDLER_H__
#define JSON_HANDLER_H__

#include <string>
#include <map>
#include <vector>
#include <rapidjson\document.h>
#include "utils.h"

class JsonHandler
{
public:
	static rapidjson::Document& LoadJsonFromFile(std::string filename);
	static rapidjson::Value LoadEntities(int map_id);
	static rapidjson::Value LoadQueues(int map_id, int entity_id);
	static rapidjson::Value LoadMap(int map_id);
	static rapidjson::Value LoadMaps();
	static void SetFile(std::string filename);
	static bool JsonHandler::DocumentNotNull();
public:
	static rapidjson::Document DataDocument;
private:
	static std::string File;
};

#endif // !JSON_HANDLER_H__
