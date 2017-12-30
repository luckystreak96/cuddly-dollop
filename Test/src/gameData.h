#ifndef GAME_DATA_H__
#define GAME_DATA_H__

#include <map>
#include <cstdio>
#include <rapidjson\writer.h>
#include <rapidjson\document.h>
#include <rapidjson\rapidjson.h>
#include <rapidjson\filewritestream.h>
#include <memory>
#include "utils.h"
#include "eventQueue.h"
#include <variant>

typedef std::map<std::string, std::variant<bool, float, int, std::string>> OptionMap;

class GameData
{
public:
	static std::string Get(std::string key);
	static void LoadFromFile();
	static void LoadSettings();
	static void EnsureBaseSettings();
	static void LoadGameData();
	static void SaveToFile();
	static void SaveGameData();
	static void SaveSettings();
	static void EmplaceFlag(std::string name, int value);
	static bool RespectsCondition(std::shared_ptr<EventQueue> ev);
	
public:
	static std::string PlayerSprite;

	// Game flags
	static std::map<std::string, int> Flags;
	
	// Game string variables
	static std::map<std::string, std::string> Strings;

	// Options
	static OptionMap Options;
	
private:
	static rapidjson::Document m_document;
	static std::string m_file;
};

#endif // !GAME_DATA_H__
