#ifndef BATTLE_ANIMATION_H__
#define BATTLE_ANIMATION_H__

#include <memory>

class BattleAnimation;
typedef std::shared_ptr<BattleAnimation> Anim_ptr;

class BattleAnimation
{
public:
	BattleAnimation() {}
	virtual void Update() {}

public:
	bool _done;
};

#endif // !BATTLE_ANIMATION_H__
