#ifndef FONT_GRAPHICS_COMPONENT
#define FONT_GRAPHICS_COMPONENT

#include "graphicsComponent.h"

class FontGraphicsComponent : public GraphicsComponent 
{
public:
	void Draw(bool withTex = true) {
		m_trans.SetOrthoProj(&OrthoProjInfo::GetRegularInstance());
		m_trans.SetTranslation(OrthoProjInfo::GetRegularInstance().Left, OrthoProjInfo::GetRegularInstance().Bottom, 0);
		Effect::SetWorldPosition(&m_trans.GetWOTransNoTranslate().m[0][0]);
		GraphicsComponent::Draw(withTex);
	}
private:
	static Transformation m_trans;
};

#endif // !FONT_GRAPHICS_COMPONENT
