#include "localizationData.h"

#include "utils.h"

#include <fstream>
#include <ostream>

std::map<std::string, std::map<std::string, std::string>> LocalizationData::Localization = std::map<std::string, std::map<std::string, std::string>>();
std::string LocalizationData::Language = "french";

// Load the localization strings
void LocalizationData::LoadLocalization()
{
	std::string localizationPath = "res/data/strings/";
	std::string file;
	rapidjson::Document document;

#ifdef NDEBUG
	std::ifstream my_file(localizationPath + "localization");
#else
	std::ifstream my_file(localizationPath + "localization.json");
#endif
	bool exists = my_file.good();
	if (!exists)
		return;
#ifdef NDEBUG
	file = Utils::ReadFile(localizationPath + "localization");
#else
	file = Utils::ReadFile(localizationPath + "localization.json");
#endif
	document.Parse(file.c_str());

	auto strings = document.GetObject();
	for (auto& v : strings)
		//for (Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr)
	{
		std::map<std::string, std::string> trans;
		auto l = v.value.GetObject();
		for (Value::ConstMemberIterator itr = l.MemberBegin(); itr != l.MemberEnd(); ++itr)
			trans.emplace(itr->name.GetString(), itr->value.GetString());
		Localization.emplace(v.name.GetString(), trans);
	}
}

std::string _(std::string key)
{
	if (LocalizationData::Localization.count(key))// && (GameData::Language == "english" || GameData::Localization.at(key).count(GameData::Language)))
	{
		//if (GameData::Language == "english")
		if (LocalizationData::Localization.at(key).count(LocalizationData::Language))
			return LocalizationData::Localization.at(key).at(LocalizationData::Language);
		else
			return key;
	}

	return key;
}
