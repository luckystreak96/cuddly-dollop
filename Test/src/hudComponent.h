#ifndef HUD_COMPONENT_H__
#define HUD_COMPONENT_H__

#include "observer.h"

class HudComponent : public Observer
{
public:
	virtual void Update();
	virtual void AdjustPosition();
};

#endif // !HUD_COMPONENT_H__
