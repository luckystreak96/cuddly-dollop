#ifndef ANIM_PARTICLE_EFFECT_H__
#define ANIM_PARTICLE_EFFECT_H__

#include "battleAnimation.h"
#include "vector3f.h"

class anim_particle_effect : public BattleAnimation
{
public:
	// Async = true
	anim_particle_effect(int amount, Vector3f colour, Vector3f* position, std::string sprite);
	virtual void Update();

private:
	int m_amount;
	Vector3f m_colour;
	Vector3f* m_position;
	std::string m_sprite;
};

#endif // !BATTLE_ANIMATION_H__
