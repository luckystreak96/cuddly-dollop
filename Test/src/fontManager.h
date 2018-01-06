#ifndef FONT_MANAGER_H__
#define FONT_MANAGER_H__

#include <map>
#include "font.h"
#include "dialogueBox.h"

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
	void SetScale(unsigned int font, float xScale, float yScale);
	void SetTextSpeed(unsigned int font, double speed);
	void ChangeLetter(unsigned int font, unsigned int index, char newChar);
	void SetText(unsigned int font, std::string text = " ", Vector3f location = Vector3f(), bool centered = false);
	unsigned int AddFont(bool sTatic = false, bool temporary = false, bool lightspeed = false, std::string texPath = "res/fonts/basic.png");
	void DisableFont(unsigned int font);
	void EnableFont(unsigned int font);
	void RemoveFont(unsigned int font);
	std::shared_ptr<Font> GetFont(unsigned int key);
	void ClearFonts();
	bool IsEmpty();

	// Generates a random key for the map
	unsigned int GenerateKey();
private:
	FontManager();
	~FontManager();
	std::map<unsigned int, std::shared_ptr<Font>> m_fonts;
};

#endif // !FONT_MANAGER_H__
