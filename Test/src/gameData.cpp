#include "gameData.h"

#include <fstream>
#include <ostream>
#include <GLFW/glfw3.h>
#include "localizationData.h"
#include "fighterFactory.h"
#include "skill.h"
#include "battleData.h"

#undef GetObject

using namespace rapidjson;

bool GameData::Loading = true;
std::string GameData::PlayerSprite = "res/sprites/entities/entity_ghost_blue.png";
std::map<std::string, int> GameData::Flags = std::map<std::string, int>();
std::map<std::string, std::string> GameData::Strings = std::map<std::string, std::string>();
std::map<std::string, Vector3f> GameData::Positions = std::map<std::string, Vector3f>();
OptionMap GameData::Options = OptionMap();
std::map<InputAction, unsigned int> GameData::KeyMap = std::map<InputAction, unsigned int>();

rapidjson::Document GameData::m_document;
std::string GameData::m_file;
std::string GameData::m_path = "res/data/saves/";

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
	LocalizationData::LoadLocalization();
	LoadGameData();
	LoadSettings();
}

void GameData::LoadGameData()
{
	Flags = std::map<std::string, int>();

	//struct stat buffer;

#ifdef NDEBUG
	//bool exists = (stat((m_path + "save").c_str(), &buffer) == 0);
	std::ifstream my_file(m_path + "save");
#else
	//bool exists = (stat((m_path + "save.json").c_str(), &buffer) == 0);
	std::ifstream my_file(m_path + "save.json");
#endif
	bool exists = my_file.good();
	if (!exists)
		return;
#ifdef NDEBUG
	m_file = Utils::ReadFile(m_path + "save");
#else
	m_file = Utils::ReadFile(m_path + "save.json");
#endif
	m_document.Parse(m_file.c_str());

	// Get the flags
	if (m_document.HasMember("flags") && m_document["flags"].IsArray())
	{
		auto flags = m_document["flags"].GetArray();
		for (auto& v : flags)
			for (Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr)
				Flags.emplace(itr->name.GetString(), itr->value.GetInt());
	}

	Strings = std::map<std::string, std::string>();
	// Get the strings
	if (m_document.HasMember("strings") && m_document["strings"].IsArray())
	{
		auto strings = m_document["strings"].GetArray();
		for (auto& v : strings)
			for (Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr)
				Strings.emplace(itr->name.GetString(), itr->value.GetString());
	}

	Positions = std::map<std::string, Vector3f>();
	// Get the positions
	if (m_document.HasMember("positions") && m_document["positions"].IsArray())
	{
		auto positions = m_document["positions"].GetArray();
		for (auto& v : positions)
			for (Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr)
			{
				std::string name = itr->name.GetString();
				Vector3f result;
				if (itr->value.HasMember("x"))
					result.x = itr->value["x"].GetFloat();
				if (itr->value.HasMember("y"))
					result.y = itr->value["y"].GetFloat();
				if (itr->value.HasMember("z"))
					result.z = itr->value["z"].GetFloat();

				Positions.emplace(name, result);
			}
	}

	// Load stat curves to be used by actors
	BattleData::LoadAll();

	// Get the party
	BattleData::LoadParty(m_document);


	PlayerSprite = "res/sprites/entities/entity_ghost.png";
	// Get the player sprite
	if (m_document.HasMember("sprite") && m_document["sprite"].IsString())
		PlayerSprite = m_document["sprite"].GetString();

	Loading = true;
}

void GameData::LoadSettings()
{
	Options = OptionMap();

#ifdef NDEBUG
	std::ifstream my_file(m_path + "config");
#else
	std::ifstream my_file(m_path + "config.json");
#endif
	bool exists = my_file.good();
	if (!exists)
	{
		EnsureBaseSettings();
		return;
	}
#ifdef NDEBUG
	m_file = Utils::ReadFile(m_path + "config");
#else
	m_file = Utils::ReadFile(m_path + "config.json");
#endif
	m_document.Parse(m_file.c_str());

	// Get the options
	if (m_document.HasMember("options") && m_document["options"].IsArray())
	{
		auto options = m_document["options"].GetArray();
		for (auto& v : options)
			for (Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr)
			{
				std::variant<bool, float, int, std::string> sec = -123456;
				std::string name = itr->name.GetString();
				if (itr->value.IsBool())
				{
					//if (name == "mute")
					//	sec = itr->value.GetBool();
					//else if (name == "fullscreen")
					//	sec = itr->value.GetBool();
					sec = itr->value.GetBool();
				}
				else if (itr->value.IsInt())
				{
					//if (name == "resolution")
					//	sec = itr->value.GetInt();
					//else
					sec = itr->value.GetInt();
				}

				// If the key isnt defined, just skip
				else
					continue;

				Options.emplace(name, sec);
			}
	}

	// Get the keybindings
	if (m_document.HasMember("keybindings") && m_document["keybindings"].IsArray())
	{
		auto bindings = m_document["keybindings"].GetArray();
		for (auto& v : bindings)
			for (Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr)
			{
				// Get the key name
				std::string name = itr->name.GetString();
				if (InputManager::StringToInputAction(name) == A_Last)
					continue;

				// Get the key value
				unsigned int sec = 0;
				if (itr->value.IsUint())
					sec = itr->value.GetUint();
				// If the key isnt defined, just skip
				else
					continue;

				KeyMap.emplace(InputManager::StringToInputAction(name), sec);
			}
	}

	EnsureBaseSettings();
}

void GameData::NewGame()
{
	BattleData::NewGame();
}

void GameData::EnsureBaseSettings()
{
	if (!Options.count("mute"))
		Options.emplace("mute", false);
	if (!Options.count("fullscreen"))
		Options.emplace("fullscreen", false);
	if (!Options.count("resolution"))
		Options.emplace("resolution", 3);
	if (!Options.count("effect_quality"))
		Options.emplace("effect_quality", 0);
	if (!Options.count("3d"))
		Options.emplace("3d", false);
	if (!Strings.count("name"))
		Strings.emplace("name", "Yanik");

	if (!KeyMap.count(A_Accept))
		KeyMap.emplace(A_Accept, GLFW_KEY_X);
	if (!KeyMap.count(A_Cancel))
		KeyMap.emplace(A_Cancel, GLFW_KEY_Z);
	if (!KeyMap.count(A_Exit))
		KeyMap.emplace(A_Exit, GLFW_KEY_ESCAPE);
	if (!KeyMap.count(A_Mute))
		KeyMap.emplace(A_Mute, GLFW_KEY_M);
	if (!KeyMap.count(A_Pause))
		KeyMap.emplace(A_Pause, GLFW_KEY_P);
	if (!KeyMap.count(A_Up))
		KeyMap.emplace(A_Up, GLFW_KEY_UP);
	if (!KeyMap.count(A_Down))
		KeyMap.emplace(A_Down, GLFW_KEY_DOWN);
	if (!KeyMap.count(A_Right))
		KeyMap.emplace(A_Right, GLFW_KEY_RIGHT);
	if (!KeyMap.count(A_Left))
		KeyMap.emplace(A_Left, GLFW_KEY_LEFT);
	if (!KeyMap.count(A_Menu))
		KeyMap.emplace(A_Menu, GLFW_KEY_ENTER);
	if (!KeyMap.count(A_AltR))
		KeyMap.emplace(A_AltR, GLFW_KEY_D);
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

	// Save positions
	Value positions_v(kArrayType);
	for (auto x : Positions)
	{
		Value ob(kObjectType);
		Value first(kStringType);
		first.SetString(StringRef(x.first.c_str()), allocator);
		Value vector(kObjectType);
		Value firstx(kStringType);
		Value secondx;
		Value firsty(kStringType);
		Value secondy;
		Value firstz(kStringType);
		Value secondz;
		firstx.SetString(StringRef("x\0"), allocator);
		secondx.SetFloat(x.second.x);
		firsty.SetString(StringRef("y\0"), allocator);
		secondy.SetFloat(x.second.y);
		firstz.SetString(StringRef("z\0"), allocator);
		secondz.SetFloat(x.second.z);
		vector.AddMember(firstx, secondx, allocator);
		vector.AddMember(firsty, secondy, allocator);
		vector.AddMember(firstz, secondz, allocator);

		ob.AddMember(first, vector, allocator);
		positions_v.PushBack(ob, allocator);
	}

	saveFile.AddMember("positions", positions_v, allocator);

	// Save party data
	BattleData::SaveParty(saveFile, allocator);

	// Save ot file
#ifdef NDEBUG
	FILE* fp = fopen((m_path + "save").c_str(), "wb"); // non-Windows use "w"
#else
	FILE* fp = fopen((m_path + "save.json").c_str(), "wb"); // non-Windows use "w"
#endif
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

	// Save basic options
	Value options(kArrayType);
	for (auto x : Options)
	{
		Value ob(kObjectType);
		Value first(kStringType);
		first.SetString(StringRef(x.first.c_str()), allocator);
		Value second;

		// Checks if the type of the object in the variant is the right one - returns null if it isn't
		if(auto val = std::get_if<bool>(&x.second))
			second.SetBool(std::get<bool>(x.second));
		else if(auto val = std::get_if<int>(&x.second))
			second.SetInt(std::get<int>(x.second));

		//if (x.first == "mute")
		//	second.SetBool(std::get<bool>(x.second));
		//else if (x.first == "fullscreen")
		//	second.SetBool(std::get<bool>(x.second));
		//else if (x.first == "resolution")
		//	second.SetInt(std::get<int>(x.second));

		ob.AddMember(first, second, allocator);
		options.PushBack(ob, allocator);
	}

	configFile.AddMember("options", options, allocator);

	// Save KeyBindings
	Value keybindings(kArrayType);
	for (auto x : KeyMap)
	{
		Value ob(kObjectType);
		Value first(kStringType);
		first.SetString(StringRef(InputManager::InputActionStrings[x.first].c_str()), allocator);
		Value second;

		second.SetUint(x.second);

		ob.AddMember(first, second, allocator);
		keybindings.PushBack(ob, allocator);
	}

	configFile.AddMember("keybindings", keybindings, allocator);

#ifdef NDEBUG
	FILE* fp = fopen((m_path + "config").c_str(), "wb"); // non-Windows use "w"
#else
	FILE* fp = fopen((m_path + "config.json").c_str(), "wb"); // non-Windows use "w"
#endif
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
