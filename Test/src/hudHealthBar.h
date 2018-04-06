#ifndef HUD_HEALTH_BAR_H__
#define HUD_HEALTH_BAR_H__

#include "hudBar.h"

class Actor;

class HudHealthBar : public HudBar
{
public:
	// Don't forget to tell the Observable that you're an observer
	HudHealthBar(Actor* ap, Vector3f position);
	void Update();
	void AdjustPosition();
	void Destroy();
	void SetRender();

private:
	// xp variables
	int m_observedXP;
	int m_xpMax;
	int m_prevXP;

	// Health bar images
	GraphComp_ptr _background;
	GraphComp_ptr _xpBar;

	// Font ID's
	unsigned int _healthFont;
	unsigned int _nameFont;
};

#endif // !HUD_HEALTH_BAR_H__
