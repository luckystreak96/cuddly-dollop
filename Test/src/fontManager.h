#ifndef FONT_MANAGER_H__
#define FONT_MANAGER_H__

#include <map>
#include "font.h"

class FontManager
{
public:
	static FontManager& GetInstance()
	{
		static FontManager fontMngr;
		return fontMngr;
	}
	void Update(double ElapsedTime);
	void SetRender();
	void SetScale(unsigned int font, Vector3f scale);
	void ChangeLetter(unsigned int font, unsigned int index, char newChar);
	void SetText(unsigned int font, std::string text = " ", Vector3f location = Vector3f(), bool centered = false);
	unsigned int AddFont(bool sTatic = false, bool temporary = false, std::string texPath = "res/fonts/basic.png");
	void RemoveFont(unsigned int font) { m_fonts.erase(font); }
	Font* GetFont(unsigned int key);
private:
	FontManager();
	~FontManager();
	std::map<unsigned int, Font*> m_fonts;
};

#endif // !FONT_MANAGER_H__
