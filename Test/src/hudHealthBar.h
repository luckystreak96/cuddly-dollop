#ifndef HUD_HEALTH_BAR_H__
#define HUD_HEALTH_BAR_H__

#include "hudComponent.h"
#include "graphicsComponent.h"

//enum HUDComponent { HC_HealthBar, HC_Text };
class Actor;

class HudHealthBar : public HudComponent
{
public:
	// Don't forget to tell the Observable that you're an observer
	HudHealthBar(Actor* ap, Vector3f position);
	void Update();
	void AdjustPosition();
	void Destroy();
	void SetRender();

private:
	int* m_observed;
	int m_prevValue;
	int m_maxHealth;

	//HUDComponent _type;

	// Health bar images
	GraphComp_ptr _foreground;
	GraphComp_ptr _background;

	// Font ID's
	unsigned int _healthFont;
	unsigned int _nameFont;
};

#endif // !HUD_HEALTH_BAR_H__
