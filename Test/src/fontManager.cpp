#include "fontManager.h"

FontManager::FontManager() : m_fonts(std::map<unsigned int, Font*>())
{
}

FontManager::~FontManager()
{
	for (auto x : m_fonts)
	{
		delete x.second;
	}
}

void FontManager::SetScale(unsigned int font, Vector3f scale)
{
	Font* temp = GetFont(font);
	if (temp == NULL)
		return;

	temp->GetGraphics()->SetScale(scale);
}

void FontManager::SetText(unsigned int font, std::string text, Vector3f location, bool centered)
{
	Font* temp = GetFont(font);
	if (temp == NULL)
		return;

	temp->SetText(text, location, centered);
}

void FontManager::SetRender()
{
	for (auto x : m_fonts)
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
		delete m_fonts.at(x);
		m_fonts.erase(x);
	}
}

unsigned int FontManager::AddFont(bool sTatic, bool temporary, std::string texPath)
{
	unsigned int key;
	while (true)
	{
		srand(clock());
		key = rand();
		if (m_fonts.count(key) == 0)
			break;
	}
	m_fonts.emplace(key, new Font(sTatic, temporary, texPath));
	return key;
}

Font* FontManager::GetFont(unsigned int key)
{
	if (m_fonts.count(key) > 0) 
		return m_fonts.at(key);
	else 
		return NULL;
}
