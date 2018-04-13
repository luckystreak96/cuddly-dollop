#ifndef FONT_FLOAT_H__
#define FONT_FLOAT_H__

#include "font.h"

class FontFloat : public Font
{
public:
	// Lifetime in seconds
	FontFloat(double lifetime, std::string tex = "res/fonts/lowercase.png");
	void SetText(std::string text, Vector3f location = Vector3f(0, 0, -1), bool centered = false, float xBoundry = -1);
	void UpdateModel();
	void Update(double elapsedTime);

public:
	Vector3f Color;
	float Alpha;
};

#endif