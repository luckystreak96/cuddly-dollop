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
	static rapidjson::Value LoadEntities();
	static rapidjson::Value LoadQueues(int entity_id);
public:
	static rapidjson::Document DataDocument;
};

#endif // !JSON_HANDLER_H__
