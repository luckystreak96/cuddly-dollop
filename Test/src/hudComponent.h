#ifndef HUD_COMPONENT_H__
#define HUD_COMPONENT_H__

#include "observer.h"

class HudComponent;
typedef std::shared_ptr<HudComponent> HudComp_ptr;

class HudComponent : public Observer
{
public:
	virtual void Update();
	virtual void AdjustPosition();
	virtual void Destroy();
	virtual void SetRender();
};

#endif // !HUD_COMPONENT_H__
