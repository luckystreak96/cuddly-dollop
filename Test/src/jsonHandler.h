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
	JsonHandler();
	rapidjson::Document& LoadJsonFromFile(std::string filename);
	rapidjson::Value LoadEntities(int map_id);
	rapidjson::Value LoadQueues(int map_id, int entity_id);
	rapidjson::Value LoadMap(int map_id);
	rapidjson::Value LoadMaps();
	void SetFile(std::string filename);
	bool JsonHandler::DocumentNotNull();
public:
	rapidjson::Document ReferenceDocument;
	rapidjson::Document DataDocument;
private:
	std::string File;
};

#endif // !JSON_HANDLER_H__
