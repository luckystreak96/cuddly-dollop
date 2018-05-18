#ifndef MENU_OPTIONS_H__
#define MENU_OPTIONS_H__

#include <vector>

#include "vector3f.h"
#include "fontGraphicsComponent.h"

class MenuOptions
{
public:
	MenuOptions();
	MenuOptions(std::vector<std::string> options, Vector3f position);

	// Returns true if a choice was selected
	bool Update();

	void SetRender();

public:
	int _selectedOption;

private:
	std::vector<std::string> m_options;
	bool m_firstUpdate;
	std::vector<std::shared_ptr<FontGraphicsComponent>> m_graphics;
	Vector3f m_position;
};

#endif