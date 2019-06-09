#ifndef ANIM_BONUS_EFFECT_H__
#define ANIM_BONUS_EFFECT_H__

#include "battleAnimation.h"
#include "vector3f.h"
#include "actor.h"

class AnimBonusEffect : public BattleAnimation
{
public:
	AnimBonusEffect(Actor_ptr owner, Actor_ptr target, double delay = 0.35);
	virtual void Update();

public:
	Actor_ptr _owner;
	Actor_ptr _target;
};

#endif // !BATTLE_ANIMATION_H__
