#include "menuFactory.h"

#include "font.h"

Font MenuFactory::BuildOption(OptionType type, std::string text, Vector3f location, bool centered)
{
	if (type == OT_Text)
	{
		Font font = Font(true, false, true);
		font.SetScale(0.5f, 0.5f);
		font.SetText(text, location, centered);
		font.Update(100);
		return font;
	}
	else
	{
		Font font = Font(true, false, true);
		font.SetScale(0.5f, 0.5f);
		font.SetText("MenuFactory::BuildOption error - OptionType not supported", location, centered);
		font.Update(100);
		return font;
	}
}

void MenuFactory::UpdateTextOption(std::string text, Vector3f location, bool centered, Font* font)
{
	font->SetScale(0.5f, 0.5f);
	font->SetText(text, location, centered);
	font->Update(100);
}

void MenuFactory::UpdateOptions(std::vector<std::string> text, Vector3f location, bool centered, std::vector<Font>* fonts)
{
	for (int i = 0; i < fonts->size(); i++)
		UpdateTextOption(text[i], location - Vector3f(0, i * 64.0f, 0), centered, &fonts->at(i));
}

std::vector<Font> MenuFactory::BuildOptions(OptionType type, std::vector<std::string> text, Vector3f location, bool centered)
{
	std::vector<Font> result;
	for (int i = 0; i < text.size(); i++)
		result.push_back(BuildOption(type, text[i], location - Vector3f(0, i * 64.0f, 0), centered));

	return result;
}
