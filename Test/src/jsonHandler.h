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
	static rapidjson::Document LoadJsonFromFile(std::string filename);
private:

};

#endif // !JSON_HANDLER_H__
