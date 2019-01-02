#ifndef HUD_STATUS_EFFECT_H__
#define HUD_STATUS_EFFECT_H__

#include "hudBar.h"
#include "battleAnimation.h"
#include "skill.h"
#include "battleUnit.h"

class HudStatusEffect : public HudComponent
{
public:
	// Don't forget to tell the Observable that you're an observer
	HudStatusEffect(BattleUnit u);
	void Update(Observable* obs);
	void AdjustPosition();
	void Destroy();
	void SetRender();
	void ToggleDisplay(bool display);
	bool GetDisplay();
	void SetBoxPosition();
	Vector3f CalculateTextPosition();

	// Remember who you're showing
	BattleUnit _unit;
	unsigned int _turnFont;

private:
	// maxhp variables
	int m_observedOrder;
	int m_prevOrder;
	Vector3f m_prevActorPos;

	// AttackType image
	GraphComp_ptr _background;

	// Font ID's

	bool m_mustDisplayNextUpdate;
	bool m_mustDisplay;
};

#endif // !HUD_STATUS_EFFECT_H__
