#ifndef ANIM_COLOR_FLASH_H__
#define ANIM_COLOR_FLASH_H__

#include "battleAnimation.h"
#include "vector3f.h"
#include "actor.h"

class AnimColorFlash : public BattleAnimation
{
public:
	// Async = true
	AnimColorFlash(Vector3f color, Actor_ptr target, double duration = 0.2);
	virtual void Update();

public:
	Actor_ptr _target;
	Vector3f _color;
	Vector3f _prevColor;
	Lerper _lerp;
};

#endif // !BATTLE_ANIMATION_H__
