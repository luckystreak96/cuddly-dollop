#ifndef GAME_DATA_H__
#define GAME_DATA_H__

#include "utils.h"
#include "eventQueue.h"
//#include "actor.h"
#include "vector3f.h"
#include "input_manager.h"

#include <variant>
#include <map>
#include <cstdio>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/filewritestream.h>
#include <memory>

typedef std::map<std::string, std::variant<bool, float, int, std::string>> OptionMap;

class GameData
{
public:
	static std::string Get(std::string key);
	static void LoadFromFile();
	static void LoadSettings();
	static void EnsureBaseSettings();
	static void NewGame();
	static void LoadGameData();
	static void SaveToFile();
	static void SaveGameData();
	static void SaveSettings();
	static void EmplaceFlag(std::string name, int value);
	static bool RespectsCondition(std::shared_ptr<EventQueue> ev);
	
public:
	// Do we load stuff like position upon entering a new map
	static bool Loading;

	// Sprite
	static std::string PlayerSprite;

	// Game flags
	static std::map<std::string, int> Flags;
	
	// Game string variables
	static std::map<std::string, std::string> Strings;

	// Game position variables
	static std::map<std::string, Vector3f> Positions;

	// Options
	static OptionMap Options;

	// Key mapping settings
	static std::vector<unsigned int> KeyMap;

	//static std::string Language;
	
private:
	static rapidjson::Document m_document;
	static std::string m_file;
	static std::string m_path;
};

#endif // !GAME_DATA_H__
