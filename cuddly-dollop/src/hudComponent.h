#ifndef HUD_COMPONENT_H__
#define HUD_COMPONENT_H__

#include "observer.h"

//class HudComponent;

class HudComponent : public Observer
{
public:
	HudComponent();
	~HudComponent();
	virtual void Update();
	virtual void Destroy();
	virtual void SetRender();

private:
	static int counter;

};

typedef std::shared_ptr<HudComponent> HudComp_ptr;

#endif // !HUD_COMPONENT_H__
