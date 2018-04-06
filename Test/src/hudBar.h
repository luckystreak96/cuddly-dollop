#ifndef HUD_BAR_H__
#define HUD_BAR_H__

#include "hudComponent.h"
#include "graphicsComponent.h"

class HudBar : public HudComponent
{
public:
	// Don't forget to tell the Observable that you're an observer
	HudBar(int* ap, int max, Vector3f position);
	virtual void Update();
	virtual void AdjustPosition();
	virtual void Destroy();
	virtual void SetRender();

protected:
	// Dont use this
	HudBar() {};

protected:
	int* m_observed;
	int m_prevValue;
	int m_max;

	// Health bar image
	GraphComp_ptr _foreground;
};

#endif // !HUD_BAR_H__
