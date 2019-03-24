#include "fontManager.h"
#include "fontFloat.h"

FontManager::FontManager() : m_fonts(std::map<unsigned int, Font_ptr>())
{
	srand(666);
}

FontManager::~FontManager()
{
	ClearFonts();
}


Font_ptr FontManager::CreateFloatingText(Vector3f target, std::string text, Vector3f color)
{
	Vector3f pos;
	Font_ptr font;

	// Setup font
	pos = target + Vector3f(0.5f, 1.f, 0);
	pos.z = 0;

	// create font
	font = Font_ptr(new FontFloat(0.75));
	font->SetText(text, pos, true);

	// color
	dynamic_cast<FontFloat*>(font.get())->Color = color;

	FontManager::GetInstance().AddFont(font);

	return font;
}

Font_ptr FontManager::CreateFloatingText(Vector3f target, std::string text)
{
	return CreateFloatingText(target, text, Vector3f(0, 0.4f, 0.9f));
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
	Font_ptr temp = GetFont(font);
	if (temp == nullptr)
		return;

	temp->SetScale(xScale, yScale);
}

void FontManager::SetText(unsigned int& font, std::string text, Vector3f location, bool centered)
{
	Font_ptr temp = GetFont(font);
	if (temp == nullptr)
		return;

	temp->SetText(text, location, centered);
}

void FontManager::SetTextSpeed(unsigned int font, double speed)
{
	Font_ptr temp = GetFont(font);
	if (temp == nullptr)
		return;

	temp->SetTextSpeed(speed);
}


void FontManager::ChangeLetter(unsigned int font, unsigned int index, char newChar)
{
	Font_ptr temp = GetFont(font);
	if (temp == nullptr)
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
		//if (OrthoProjInfo::GetRegularInstance().changed)
		//	x.second->UpdateModel();
			//x.second->Reset();
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
	Font_ptr font = Font_ptr(new Font(sTatic, temporary, lightspeed, texPath));
	return AddFont(font);
}

unsigned int FontManager::AddFont(Font_ptr font)
{
	unsigned int key;
	while (true)
	{
		key = rand();
		if (m_fonts.count(key) == 0)
			break;
	}
	m_fonts.emplace(key, font);
	return key;
}

void FontManager::EnableFont(unsigned int font)
{
	if (m_fonts.count(font))
		m_fonts.at(font)->_enabled = true;
}

void FontManager::DisableFont(unsigned int font)
{
	if (m_fonts.count(font))
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


Font_ptr FontManager::GetFont(unsigned int key)
{
	if (m_fonts.count(key) > 0)
		return m_fonts.at(key);
	else
		return nullptr;
}

void FontManager::UpdateText(unsigned int &font, std::string text) {

	Font_ptr temp = GetFont(font);
	if (temp == nullptr)
		return;

	temp->UpdateText(text);
}
