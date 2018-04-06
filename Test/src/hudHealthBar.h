#ifndef HUD_HEALTH_BAR_H__
#define HUD_HEALTH_BAR_H__

#include "hudBar.h"
#include "battleAnimation.h"

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
	Anim_ptr SetupExpAnimation(int targetXP);
	bool UpdateExpAnimation(float newxp);


	// Remember who you're showing
	Actor* _actor;

private:
	// maxhp variables
	int* m_observedMaxHP;
	int m_prevMaxHP;

	// xp variables
	int m_observedXP;
	int m_xpMax;
	int m_prevXP;

	// xp animation variables
	int m_startXP;
	int m_targetXP;
	int m_currentLevel;
	int m_targetLevel;

	// Health bar images
	GraphComp_ptr _background;
	GraphComp_ptr _xpBar;

	// Font ID's
	unsigned int _healthFont;
	unsigned int _nameFont;
	unsigned int _levelFont;
};

#endif // !HUD_HEALTH_BAR_H__
