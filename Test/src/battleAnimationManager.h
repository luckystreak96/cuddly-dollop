#ifndef BATTLE_ANIMATION_MANAGER_H__
#define BATTLE_ANIMATION_MANAGER_H__

#include <map>
#include "skill.h"
#include "actor.h"
#include "mathutils.h"
#include "battleAnimation.h"

class BattleAnimationManager
{
public:
	BattleAnimationManager();

	const std::map<int, Actor_ptr>& GetActors() { return m_actors; }

	Anim_ptr CreateAnimation(AnimationOperation ao);
	void UpdateColors(int fighterid, bool selected = false, bool dead = false);
	void MoveUp(int fighterid, bool foreward);
	virtual void SpawnDamageText(Actor_ptr target, Damage dmg, Skill_ptr skill);
	virtual void SpawnStatusText(Actor_ptr target, std::string statusName);
	void CreateFloatingText(int fighterid, std::string text);

	// Animations
	void UpdateAnimations();
	int GetAnimationsSize() { return m_animations.size(); }
	bool Animating();
	void Push_Back_Animation(Anim_ptr anim);

private:
	std::map<int, Actor_ptr> m_actors;
	std::deque<Anim_ptr> m_animations;
};

#endif