#ifndef HUD_HEALTH_BAR_H__
#define HUD_HEALTH_BAR_H__

#include "hudComponent.h"

class HudHealthBar : public HudComponent
{
public:
	// Don't forget to add the pointer to the Observable as well
	HudHealthBar(int* observable);
	void Update();
	void AdjustPosition();

private:
	int* m_observed;
	int m_prevValue;
};

#endif // !HUD_HEALTH_BAR_H__
