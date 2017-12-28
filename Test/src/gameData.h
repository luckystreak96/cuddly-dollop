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

class GameData
{
public:
	static void LoadFromFile();
	static void SaveToFile();
	static void SaveGameData();
	static void SaveSettings();
	static void EmplaceFlag(std::string name, int value);
	static bool RespectsCondition(std::shared_ptr<EventQueue> ev);
	
public:
	static std::string PlayerSprite;
	static std::map<std::string, int> Flags;
	//static std::map<std::string, int> Flags;
	
	
private:
	static rapidjson::Document m_document;
	static std::string m_file;
};

#endif // !GAME_DATA_H__
