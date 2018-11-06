#ifndef MENU_FACTORY_H__
#define MENU_FACTORY_H__

#include "fontGraphicsComponent.h"

enum OptionType { OT_Text };

class MenuFactory
{
public:
	static std::shared_ptr<GraphicsComponent> BuildOption(OptionType type, std::string text, Vector3f location, bool centered);
	static std::vector<std::shared_ptr<GraphicsComponent>> BuildOptions(OptionType type, std::vector<std::string> text, Vector3f location, bool centered);
};

#endif