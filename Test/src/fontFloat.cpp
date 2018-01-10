#include "fontFloat.h"

FontFloat::FontFloat(double lifetime, std::string tex) : Font(false, true, false, tex)
{
	m_lifetime = lifetime;
	SetScale(0.5f, 0.5f);
}

void FontFloat::SetText(std::string text, Vector3f location, bool centered, float xBoundry)
{
	location.y -= 0.2f;
	Font::SetText(text, location, centered, xBoundry);
}

void FontFloat::Update(double elapsedTime)
{
	m_offset.y += elapsedTime * 0.8;
	UpdateModel();
	Font::Update(elapsedTime);
}

void FontFloat::UpdateModel()
{
	if (m_graphics)
	{
		m_graphics->GetMModels().clear();
		for (auto x : m_letterPositions)
		{
			Transformation t;
			t.SetTranslation(x + m_offset);
			t.SetScale(Vector3f(m_xScale, m_yScale, 1));
			m_graphics->InsertMModels(t);
		}
	}
}