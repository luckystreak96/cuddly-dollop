#include "gameData.h"

using namespace rapidjson;

std::string GameData::PlayerSprite = "res/sprites/entities/entity_ghost_blue.png";
std::map<std::string, int> GameData::Flags = std::map<std::string, int>();

rapidjson::Document GameData::m_document;
std::string GameData::m_file;

//GameData::GameData() : PlayerSprite("res/sprites/entities/ghost_blue.png")
//{
//}
//
//GameData::~GameData()
//{
//	SaveToFile();
//}

bool GameData::RespectsCondition(std::shared_ptr<EventQueue> ev)
{
	if (ev->Flag == "")
		return true;

	if (!Flags.count(ev->Flag))
		Flags.emplace(ev->Flag, 0);

	switch (ev->Condition)
	{
	case FlagCondition::FC_Value:
		if (Flags.at(ev->Flag) == ev->FlagValue)
			return true;
		break;
	case FlagCondition::FC_NotValue:
		if (Flags.at(ev->Flag) != ev->FlagValue)
			return true;
		break;
	case FlagCondition::FC_GreaterThan:
		if (Flags.at(ev->Flag) > ev->FlagValue)
			return true;
		break;
	case FlagCondition::FC_LessThan:
		if (Flags.at(ev->Flag) < ev->FlagValue)
			return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}


void GameData::EmplaceFlag(std::string name, int value)
{
	if (Flags.count(name))
		Flags.erase(name);

	Flags.emplace(name, value);
}


void GameData::LoadFromFile()
{
	Flags = std::map<std::string, int>();

	struct stat buffer;
	bool exists = (stat("res/data/save", &buffer) == 0);
	if (!exists)
		return;
	m_file = Utils::ReadFile("res/data/save");
	m_document.Parse(m_file.c_str());

	// Get the flags
	if (m_document.HasMember("flags") && m_document["flags"].IsArray())
	{
		auto& flags = m_document["flags"].GetArray();
		for (auto& v : flags)
			for (Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr)
				Flags.emplace(itr->name.GetString(), itr->value.GetInt());
	}

	// Get the player sprite
	if (m_document.HasMember("sprite") && m_document["sprite"].IsString())
		PlayerSprite = m_document["sprite"].GetString();
}

void GameData::SaveGameData()
{
	//============ GAME SAVE FILE ===============
	Document saveFile;
	saveFile.SetObject();

	Document::AllocatorType& allocator = saveFile.GetAllocator();

	Value flags(kArrayType);
	for (auto x : Flags)
	{
		Value ob(kObjectType);
		Value first(kStringType);
		first.SetString(StringRef(x.first.c_str()), allocator);
		Value second;
		second.SetInt(x.second);

		ob.AddMember(first, second, allocator);
		flags.PushBack(ob, allocator);
	}

	saveFile.AddMember("flags", flags, allocator);
	saveFile.AddMember("sprite", StringRef(PlayerSprite.c_str()), allocator);

	FILE* fp = fopen("res/data/save", "wb"); // non-Windows use "w"
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	saveFile.Accept(writer);
	fclose(fp);
}
void GameData::SaveSettings()
{
	//============ CONFIG SAVE FILE ===============
	Document configFile;
	configFile.SetObject();

	Document::AllocatorType& allocator = configFile.GetAllocator();

	Value flags(kArrayType);
	for (auto x : Flags)
	{
		Value ob(kObjectType);
		Value first(kStringType);
		first.SetString(StringRef(x.first.c_str()), allocator);
		Value second;
		second.SetInt(x.second);

		ob.AddMember(first, second, allocator);
		flags.PushBack(ob, allocator);
	}

	configFile.AddMember("flags", flags, allocator);
	configFile.AddMember("sprite", StringRef(PlayerSprite.c_str()), allocator);

	FILE* fp = fopen("res/data/save", "wb"); // non-Windows use "w"
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	configFile.Accept(writer);
	fclose(fp);
}

void GameData::SaveToFile()
{
	SaveGameData();
	SaveSettings();
}