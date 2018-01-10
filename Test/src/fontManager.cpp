#include "fontManager.h"

FontManager::FontManager() : m_fonts(std::map<unsigned int, std::shared_ptr<Font>>())
{
}

FontManager::~FontManager()
{
	ClearFonts();
}

bool FontManager::IsEmpty()
{
	return m_fonts.empty();
}

void FontManager::ClearFonts()
{
	m_fonts.clear();
}

void FontManager::SetScale(unsigned int font, float xScale, float yScale)
{
	std::shared_ptr<Font> temp = GetFont(font);
	if (temp == NULL)
		return;

	temp->SetScale(xScale, yScale);
}

void FontManager::SetText(unsigned int font, std::string text, Vector3f location, bool centered)
{
	std::shared_ptr<Font> temp = GetFont(font);
	if (temp == NULL)
		return;

	temp->SetText(text, location, centered);
}

void FontManager::SetTextSpeed(unsigned int font, double speed)
{
	std::shared_ptr<Font> temp = GetFont(font);
	if (temp == NULL)
		return;

	temp->SetTextSpeed(speed);
}


void FontManager::ChangeLetter(unsigned int font, unsigned int index, char newChar)
{
	std::shared_ptr<Font> temp = GetFont(font);
	if (temp == NULL)
		return;

	temp->ChangeLetter(index, newChar);
}


void FontManager::SetRender()
{
	for (auto x : m_fonts)
		if (x.second->_enabled)
			x.second->SetRender();
}

void FontManager::Update(double elapsedTime)
{
	std::vector<unsigned int> to_remove = std::vector<unsigned int>();
	for (auto x : m_fonts)
	{
		x.second->Update(elapsedTime);
		if (x.second->IsDead())
			to_remove.push_back(x.first);
	}
	for (auto x : to_remove)
	{
		m_fonts.erase(x);
	}
}

unsigned int FontManager::AddFont(bool sTatic, bool temporary, bool lightspeed, std::string texPath)
{
	std::shared_ptr<Font> font = std::shared_ptr<Font>(new Font(sTatic, temporary, lightspeed, texPath));
	return AddFont(font);
}

unsigned int FontManager::AddFont(std::shared_ptr<Font> font)
{
	unsigned int key;
	while (true)
	{
		srand(clock());
		key = rand();
		if (m_fonts.count(key) == 0)
			break;
	}
	m_fonts.emplace(key, font);
	return key;
}

void FontManager::EnableFont(unsigned int font)
{
	m_fonts.at(font)->_enabled = true;
}

void FontManager::DisableFont(unsigned int font)
{
	m_fonts.at(font)->_enabled = false;
}

void FontManager::RemoveFont(unsigned int font)
{
	if (m_fonts.size() == 0)
		return;
	if (m_fonts.count(font))
	{
		m_fonts.erase(font);
	}
}

unsigned int FontManager::GenerateKey()
{
	unsigned int key;
	while (true)
	{
		srand(clock());
		key = rand();
		if (m_fonts.count(key) == 0)
			return key;
	}
}


std::shared_ptr<Font> FontManager::GetFont(unsigned int key)
{
	if (m_fonts.count(key) > 0)
		return m_fonts.at(key);
	else
		return NULL;
}
