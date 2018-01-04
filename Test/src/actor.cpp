#include "actor.h"
#include "skill.h"

Actor::Actor() : Sprite("res/sprites/entities/entity_ghost.png"), Name("Player"),
Health(10), Dead(false), Speed(0), Team(0), Selected(false), ChoosingAction(false)
{
	Skills.push_back(std::shared_ptr<Skill>(new Skill()));
}