#ifndef SKILL_H__
#define SKILL_H__

#include <vector>
#include <deque>
#include <set>
#include <memory>
#include <iostream>
#include "actor.h"
#include "battleAnimation.h"

enum BattleState;
class Skill;
typedef std::shared_ptr<Skill> Skill_ptr;

class Skill
{
public:
	Skill();
	virtual BattleState Start(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner);
	virtual void Update();
	virtual void Reset();
	virtual bool IsReady();

public:
	std::deque<Actor_ptr>* _actors;
	std::vector<Actor_ptr>* _targets;
	std::deque<Anim_ptr>* _anims;
	std::set<int> _input;
	std::string _name;
	bool _done;
	int _animProg;
	int _cooldown;
	int _currentCooldown;
	int _minTargets;
	Actor_ptr _owner;


protected:
	void DefaultSetup();
};


#endif // !SKILL_H__
