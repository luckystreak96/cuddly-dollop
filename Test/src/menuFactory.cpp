#include "menuFactory.h"

#include "font.h"

std::shared_ptr<FontGraphicsComponent> MenuFactory::BuildOption(OptionType type, std::string text, Vector3f location, bool centered)
{
	if (type == OT_Text)
	{
		Font font = Font(true, false, true);
		font.SetScale(0.3f, 0.3f);
		font.SetText(text, location, centered);
		font.Update(100);
		return font.GetGraphics();
	}
}

std::vector<std::shared_ptr<FontGraphicsComponent>> MenuFactory::BuildOptions(OptionType type, std::vector<std::string> text, Vector3f location, bool centered)
{
	std::vector<std::shared_ptr<FontGraphicsComponent>> result;
	for (int i = 0; i < text.size(); i++)
		result.push_back(BuildOption(type, text[i], location - Vector3f(0, i, 0), centered));

	return result;
}
