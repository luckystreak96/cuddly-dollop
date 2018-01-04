#ifndef ACTOR_H__
#define ACTOR_H__

#include <string>
#include <memory>
#include <vector>

class Skill;

class Actor
{
public:
	Actor();

public:
	std::string Sprite;
	std::string Name;
	int Health;
	int Speed;
	bool Dead;
	int Team;
	bool Selected;
	bool ChoosingAction;
	std::vector<std::shared_ptr<Skill>> Skills;
	static inline bool ActorSpeedSort(std::shared_ptr<Actor> a, std::shared_ptr<Actor> b) { return a->Speed > b->Speed; }
};

#endif