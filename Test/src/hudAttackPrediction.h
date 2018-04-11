#ifndef HUD_ATTACK_PREDICTION_H__
#define HUD_ATTACK_PREDICTION_H__

#include "hudBar.h"
#include "battleAnimation.h"
#include "skill.h"

class Actor;

class HudAttackPrediction : public HudComponent
{
public:
	// Don't forget to tell the Observable that you're an observer
	HudAttackPrediction(Actor* owner);
	void Update();
	void AdjustPosition();
	void Destroy();
	void SetRender();
	void ToggleDisplay(bool display);

	// Remember who you're showing
	Actor* _actor;

private:
	// maxhp variables
	Damage m_observedDamage;
	Damage m_prevDamage;

	// AttackType image
	GraphComp_ptr _background;

	// Font ID's
	unsigned int _damageFont;

	bool m_mustDisplay;
};

#endif // !HUD_HEALTH_BAR_H__
