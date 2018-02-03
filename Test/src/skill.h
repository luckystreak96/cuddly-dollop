#ifndef SKILL_H__
#define SKILL_H__

#include <vector>
#include <deque>
#include <set>
#include <memory>
#include <iostream>
#include "actor.h"
#include "battleAnimation.h"

enum BattleState { BS_TurnStart, BS_SelectAction, BS_SelectTargets, BS_ActionProgress, BS_ActionDone, BS_TurnEnd };

class Skill;
typedef std::shared_ptr<Skill> Skill_ptr;

enum TargetMode { TM_Enemy, TM_Ally, TM_Alive, TM_Dead, TM_Any };
enum DefaultTarget { DT_Self, DT_Enemy, DT_Ally };

class Skill
{
public:
	Skill();
	virtual BattleState Start(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner);
	virtual void Update();
	virtual void Reset();
	virtual bool IsReady();
	virtual void SpawnDamageText(Actor_ptr target, int dmg);
	bool AnimationsDone();

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
	TargetMode _targetMode;
	DefaultTarget _defaultTarget;

protected:
	void DefaultSetup();
};


#endif // !SKILL_H__
