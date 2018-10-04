#ifndef BATTLE_ANIMATION_MANAGER_H__
#define BATTLE_ANIMATION_MANAGER_H__

#include <map>
#include "skill.h"
#include "actor.h"
#include "mathutils.h"
#include "battleAnimation.h"
#include "battleHUD.h"
#include "battleUnit.h"

class BattleAnimationManager
{
public:
	BattleAnimationManager();

	void SetActors(std::map<int, Actor_ptr> actors) { m_actors = actors; }
	const std::map<int, Actor_ptr>& GetActors() { return m_actors; }

	Anim_ptr CreateAnimation(triple ao);
	void UpdateColors(int fighterid, bool selected = false, bool dead = false, int actionCommandLevel = 0);
	void MoveUp(int fighterid, bool foreward);
	virtual void SpawnDamageText(Actor_ptr target, Damage dmg, Skill_ptr skill);
	virtual void SpawnStatusText(Actor_ptr target, std::string statusName);
	void CreateFloatingText(int fighterid, std::string text);

	// Misc
	Vector3f CalculateDamageColor(Skill_ptr, Damage);
	Vector3f CalculateTextColor(Skill_ptr, Damage);

	// Specific Animations
	void DamageAnimation(int target, Skill_ptr skill, Damage dmg);
	void ExpAnimation(int id, int xp);

	// Animations
	void UpdateAnimations();
	int GetAnimationsSize() { return m_animations.size(); }
	bool Animating();
	void insert_animation(Anim_ptr anim);
	double get_animation_progress();
	void set_last_animation_non_async();

	// HUD
	void SetupHUD(BattleUnit unit);
	void UpdateHUD();

	void SetRender();

public:
	int _owner;

private:
	std::map<int, Actor_ptr> m_actors;
	std::deque<Anim_ptr> m_animations;

	BattleHUD m_hud;
	int m_particleSprite;
};

#endif