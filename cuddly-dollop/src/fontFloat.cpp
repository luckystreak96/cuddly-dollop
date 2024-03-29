#include "fontFloat.h"

#include "graphics_component.h"

FontFloat::FontFloat(double lifetime, std::string tex) : Font(false, true, true, tex),
Alpha(1.0f)
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
	m_offset.y += elapsedTime * 0.8 * 64.0f;
	Alpha = 1.f - (float)(m_elapsedTime / m_lifetime);
	UpdateModel();
	m_mesh.get_graphics()->SetColorAll(Color, Alpha);
	Font::Update(elapsedTime);
}

void FontFloat::UpdateModel()
{
	if (m_mesh.get_graphics())
	{
		m_mesh.get_graphics()->get_buffers()->update_model_buffer()->clear();
		for (auto& x : m_letterPositions)
		{
			Transformation t;
			t.SetTranslation(x + m_offset);
			t.SetScale(Vector3f(m_xScale, m_yScale, 1));
			m_mesh.get_graphics()->InsertMModels(t);
		}
	}
}
