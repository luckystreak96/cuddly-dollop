#ifndef HUD_HEALTH_BAR_H__
#define HUD_HEALTH_BAR_H__

#include "hudBar.h"
#include "battleAnimation.h"
#include "battleUnit.h"

class HudHealthBar : public HudBar
{
public:
	// Don't forget to tell the Observable that you're an observer
	HudHealthBar(BattleUnit unit, Vector3f pos);
	~HudHealthBar();
	void Update();
	void Update(Observable* obs);
	void Destroy();
	void SetRender();

	// xp animation stuffs
	Anim_ptr SetupExpAnimation(int targetXP);
	bool UpdateExpAnimation(float newxp);

	// small hp bar stuffs
	void UpdateSmallHPPosition();

	// Remember who you're showing
	BattleUnit _unit;

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
	std::shared_ptr<GraphicsComponent> _background;
	std::shared_ptr<GraphicsComponent> _xpBar;

	// Smaller health bar
	Vector3f m_actorPrevPos;
	std::shared_ptr<GraphicsComponent> _smallForeground;
	std::shared_ptr<GraphicsComponent> _smallBackground;

	// Font ID's
	unsigned int _healthFont;
	unsigned int _nameFont;
	unsigned int _levelFont;
};

#endif // !HUD_HEALTH_BAR_H__
