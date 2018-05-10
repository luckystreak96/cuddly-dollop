#ifndef BATTLE_DATA_H__
#define BATTLE_DATA_H__

#include <vector>
#include <map>
#include "actor.h"
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/filewritestream.h>

#include "passiveSkill.h"

using namespace rapidjson;

class BattleData
{
public:
	static void NewGame();
	static void SaveParty(Document& doc, Document::AllocatorType& allocator);
	static void LoadParty(Document& doc);
	static void LoadAll();
	static void LoadCurves();
	static void LoadPassives();
	static void LoadActors();

public:
	// Party
	static std::vector<Actor_ptr> Party;
	static std::map<int, int> StatCurves;
	static std::map<int, Passive_ptr> PassiveSkills;
	static std::map<int, Actor_ptr> Actors;

};

#endif