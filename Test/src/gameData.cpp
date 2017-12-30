#include "gameData.h"

using namespace rapidjson;

std::string GameData::PlayerSprite = "res/sprites/entities/entity_ghost_blue.png";
std::map<std::string, int> GameData::Flags = std::map<std::string, int>();
std::map<std::string, std::string> GameData::Strings = std::map<std::string, std::string>();
OptionMap GameData::Options = OptionMap();

rapidjson::Document GameData::m_document;
std::string GameData::m_file;

std::string GameData::Get(std::string key)
{
	if (Strings.count(key))
		return Strings.at(key);
	else if (Flags.count(key))
		return std::to_string(Flags.at(key));

	return "Nothing";
}

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
	LoadGameData();
	LoadSettings();
}

void GameData::LoadGameData()
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

	// Get the strings
	if (m_document.HasMember("strings") && m_document["strings"].IsArray())
	{
		auto& strings = m_document["strings"].GetArray();
		for (auto& v : strings)
			for (Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr)
				Strings.emplace(itr->name.GetString(), itr->value.GetString());
	}

	// Get the player sprite
	if (m_document.HasMember("sprite") && m_document["sprite"].IsString())
		PlayerSprite = m_document["sprite"].GetString();
}

void GameData::LoadSettings()
{
	Options = OptionMap();

	struct stat buffer;
	bool exists = (stat("res/data/config", &buffer) == 0);
	if (!exists)
	{
		EnsureBaseSettings();
		return;
	}
	m_file = Utils::ReadFile("res/data/config");
	m_document.Parse(m_file.c_str());

	// Get the options
	if (m_document.HasMember("options") && m_document["options"].IsArray())
	{
		auto& options = m_document["options"].GetArray();
		for (auto& v : options)
			for (Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr)
			{
				std::variant<bool, float, int, std::string> sec = -123456;
				std::string name = itr->name.GetString();
				if (itr->value.IsBool())
				{
					if (name == "mute")
						sec = itr->value.GetBool();
					else if (name == "fullscreen")
						sec = itr->value.GetBool();
				}

				// If the key isnt defined, just skip
				else
					continue;

				Options.emplace(name, sec);
			}
	}
	EnsureBaseSettings();
}

void GameData::EnsureBaseSettings()
{
	if (!Options.count("mute"))
		Options.emplace("mute", false);
	if (!Options.count("fullscreen"))
		Options.emplace("fullscreen", true);
	if (!Strings.count("name"))
		Strings.emplace("name", "Yanik");
}

void GameData::SaveGameData()
{
	//============ GAME SAVE FILE ===============
	Document saveFile;
	saveFile.SetObject();

	Document::AllocatorType& allocator = saveFile.GetAllocator();

	// Save flags
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

	// Save strings
	Value strings_v(kArrayType);
	for (auto x : Strings)
	{
		Value ob(kObjectType);
		Value first(kStringType);
		first.SetString(StringRef(x.first.c_str()), allocator);
		Value second;
		second.SetString(StringRef(x.second.c_str()), allocator);

		ob.AddMember(first, second, allocator);
		strings_v.PushBack(ob, allocator);
	}

	saveFile.AddMember("strings", strings_v, allocator);

	// Sprite
	saveFile.AddMember("sprite", StringRef(PlayerSprite.c_str()), allocator);

	// Save ot file
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

	Value options(kArrayType);
	for (auto x : Options)
	{
		Value ob(kObjectType);
		Value first(kStringType);
		first.SetString(StringRef(x.first.c_str()), allocator);
		Value second;

		if (x.first == "mute")
			second.SetBool(std::get<bool>(x.second));
		else if (x.first == "fullscreen")
			second.SetBool(std::get<bool>(x.second));

		ob.AddMember(first, second, allocator);
		options.PushBack(ob, allocator);
	}

	configFile.AddMember("options", options, allocator);

	FILE* fp = fopen("res/data/config", "wb"); // non-Windows use "w"
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