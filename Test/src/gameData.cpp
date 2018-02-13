#include "gameData.h"
#include "actorFactory.h"
#include "skill.h"
#include <fstream>
#include <ostream>

#undef GetObject

using namespace rapidjson;

bool GameData::Loading = true;
std::string GameData::PlayerSprite = "res/sprites/entities/entity_ghost_blue.png";
std::vector<Actor_ptr> GameData::Party = std::vector<Actor_ptr>();
std::map<std::string, int> GameData::Flags = std::map<std::string, int>();
std::map<std::string, std::string> GameData::Strings = std::map<std::string, std::string>();
std::map<std::string, Vector3f> GameData::Positions = std::map<std::string, Vector3f>();
std::map<std::string, std::map<std::string, std::string>> GameData::Localization = std::map<std::string, std::map<std::string, std::string>>();
OptionMap GameData::Options = OptionMap();

rapidjson::Document GameData::m_document;
std::string GameData::m_file;
std::string GameData::m_path = "res/data/saves/";
std::string GameData::m_localizationPath = "res/data/strings/";
std::string GameData::Language = "french";

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
	LoadLocalization();
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

	// Get the party
	if (m_document.HasMember("party") && m_document["party"].IsArray())
	{
		auto party = m_document["party"].GetArray();
		Party = ActorFactory::BuildParty(party);
	}

	PlayerSprite = "res/sprites/entities/entity_ghost.png";
	// Get the player sprite
	if (m_document.HasMember("sprite") && m_document["sprite"].IsString())
		PlayerSprite = m_document["sprite"].GetString();

	Loading = true;
}

// Load the localization strings
void GameData::LoadLocalization()
{
#ifdef NDEBUG
	std::ifstream my_file(m_localizationPath + "localization");
#else
	std::ifstream my_file(m_localizationPath + "localization.json");
#endif
	bool exists = my_file.good();
	if (!exists)
		return;
#ifdef NDEBUG
	m_file = Utils::ReadFile(m_localizationPath + "localization");
#else
	m_file = Utils::ReadFile(m_localizationPath + "localization.json");
#endif
	m_document.Parse(m_file.c_str());

	auto strings = m_document.GetObject();
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

void GameData::NewGame()
{
	if (!Party.size() > 0)
		Party.push_back(ActorFactory::BuildBaseAlly());
}

void GameData::EnsureBaseSettings()
{
	if (!Options.count("mute"))
		Options.emplace("mute", false);
	if (!Options.count("fullscreen"))
		Options.emplace("fullscreen", false);
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
	Value party_v(kArrayType);
	for (auto actor : Party)
	{
		Value ob(kObjectType);
		Value first(kStringType);
		Value second;

		// name
		first.SetString(StringRef("name"), allocator);
		second.SetString(StringRef(actor->Name.c_str()), allocator);
		ob.AddMember(first, second, allocator);

		// health
		first.SetString(StringRef("health"), allocator);
		second.SetInt(actor->Health);
		ob.AddMember(first, second, allocator);

		// maxHealth
		first.SetString(StringRef("max_health"), allocator);
		second.SetInt(actor->GetMaxHealth());
		ob.AddMember(first, second, allocator);

		// speed
		first.SetString(StringRef("speed"), allocator);
		second.SetInt(actor->Speed);
		ob.AddMember(first, second, allocator);

		// crit
		first.SetString(StringRef("crit"), allocator);
		second.SetInt(actor->Crit);
		ob.AddMember(first, second, allocator);

		// defense 
		first.SetString(StringRef("defense"), allocator);
		second.SetInt(actor->Defense);
		ob.AddMember(first, second, allocator);

		// strength
		first.SetString(StringRef("strength"), allocator);
		second.SetInt(actor->Strength);
		ob.AddMember(first, second, allocator);

		// endurance
		first.SetString(StringRef("endurance"), allocator);
		second.SetInt(actor->GetEndurance());
		ob.AddMember(first, second, allocator);

		// dead
		first.SetString(StringRef("dead"), allocator);
		second.SetBool(actor->Dead);
		ob.AddMember(first, second, allocator);

		// skills
		first.SetString(StringRef("skills"), allocator);
		second = Value(kArrayType);
		for (auto skill : actor->Skills)
		{
			Value skl(kStringType);
			skl.SetString(StringRef(skill->_name.c_str()), allocator);
			second.PushBack(skl, allocator);
		}
		ob.AddMember(first, second, allocator);

		party_v.PushBack(ob, allocator);
	}

	saveFile.AddMember("party", party_v, allocator);

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

std::string _(std::string key)
{
	if (GameData::Localization.count(key) && (GameData::Language == "english" || GameData::Localization.at(key).count(GameData::Language)))
	{
		if (GameData::Language == "english")
			return key;
		else
			return GameData::Localization.at(key).at(GameData::Language);
	}

	return key;
}
