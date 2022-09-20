#ifndef LOCALIZATION_DATA_H__
#define LOCALIZATION_DATA_H__

#include <string>
#include <vector>
#include <map>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/filewritestream.h>

using namespace rapidjson;

class LocalizationData
{
public:
	static void LoadLocalization();

public:
	// Localization strings
	static std::map<std::string, std::map<std::string, std::string>> Localization;
	static std::string Language;

};

// Method to localize
std::string _(std::string key);

#endif