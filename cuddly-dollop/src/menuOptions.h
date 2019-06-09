#ifndef MENU_OPTIONS_H__
#define MENU_OPTIONS_H__

#include <vector>
#include <memory>

#include "vector3f.h"
#include "fontGraphicsComponent.h"
#include "font.h"
#include "command_executor.h"

class MenuOption : public CommandExecutor
{
public:
	MenuOption(std::string title);
	std::string _title;
};

class MenuOptions
{
public:
	MenuOptions();
	~MenuOptions();
	void init(std::vector<std::string> options, Vector3f position, bool centered = true);
	void init(std::vector<std::shared_ptr<MenuOption>> options, Vector3f position, bool centered = true);

	// Returns true if a choice was selected
	bool Update();

	void SetRender();
	void disable();
	void enable();

public:
	int _selectedOption;

private:
	std::vector<std::shared_ptr<MenuOption>> m_options;
	bool m_firstUpdate;
	std::vector<Font> m_fonts;
	Vector3f m_position;

	bool m_enabled;
};

#endif