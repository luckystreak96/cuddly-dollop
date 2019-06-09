#ifndef MENU_FACTORY_H__
#define MENU_FACTORY_H__

#include "fontGraphicsComponent.h"
#include "font.h"

enum OptionType { OT_Text };

class MenuFactory
{
public:
	static Font BuildOption(OptionType type, std::string text, Vector3f location, bool centered);
	static std::vector<Font> BuildOptions(OptionType type, std::vector<std::string> text, Vector3f location, bool centered);
	static void UpdateTextOption(std::string text, Vector3f location, bool centered, Font* font);
	static void UpdateOptions(std::vector<std::string> text, Vector3f location, bool centered, std::vector<Font>* fonts);
};

#endif