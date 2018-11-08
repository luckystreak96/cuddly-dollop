#ifndef MENU_OPTIONS_H__
#define MENU_OPTIONS_H__

#include <vector>

#include "vector3f.h"
#include "fontGraphicsComponent.h"
#include "font.h"

class MenuOptions
{
public:
	MenuOptions();
	void init(std::vector<std::string> options, Vector3f position);

	// Returns true if a choice was selected
	bool Update();

	void SetRender();
	void disable();
	void enable();

public:
	int _selectedOption;

private:
	std::vector<std::string> m_options;
	bool m_firstUpdate;
	std::vector<Font> m_fonts;
	Vector3f m_position;

	bool m_enabled;
};

#endif