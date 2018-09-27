#ifndef HUD_ARROW_H__
#define HUD_ARROW_H__

#include "hudComponent.h"
#include "graphicsComponent.h"
#include "battleUnit.h"

class HudArrow : public HudComponent
{
public:
	// Don't forget to tell the Observable that you're an observer
	HudArrow(BattleUnit unit, std::map<int, HudBattleUnit>* units);
	virtual void Update();
	virtual void Destroy();
	virtual void SetRender();
	void SetPosition(Vector3f start, Vector3f end);
	void ToggleHidden(bool hidden);

public:
	BattleUnit _unit;
	std::map<int, HudBattleUnit>* _units;

protected:
	Vector3f _start;
	Vector3f _end;
	bool _hidden;
	bool _showing;

	// Arrow image
	GraphComp_ptr _graphic;
};

#endif // !HUD_BAR_H__
