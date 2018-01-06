#include "skill.h"
#include "battleManager.h"

Skill::Skill()
{
	DefaultSetup();
}

void Skill::DefaultSetup()
{
	_cooldown = 0;
	_name = "SKILL?";
	_done = false;
	_currentCooldown = 0;
	_minTargets = 1;
}

// Must return the new state
BattleState Skill::Start(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner)
{
	_owner = owner;
	_targets = targets;
	_actors = actors;
	_anims = anims;
	std::cout << "Did nothing to: " << targets->at(0)->Name << std::endl;
	return BS_ActionDone;
}

void Skill::Update()
{
	// Reset input
	_input = std::set<int>();
}

void Skill::Reset()
{
	_done = false;
}

bool Skill::IsReady()
{
	return _currentCooldown <= 0;
}
