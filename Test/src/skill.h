#ifndef SKILL_H__
#define SKILL_H__

#include <vector>
#include <deque>
#include <set>
#include <memory>
#include "actor.h"
#include "battleAnimation.h"

enum BattleState;

class Skill
{
public:
	Skill();
	virtual BattleState Start(std::vector<std::shared_ptr<Actor>> targets, std::deque<std::shared_ptr<Actor>>* actors, std::deque<std::shared_ptr<BattleAnimation>>* anims);
	virtual void Update();
	virtual void Reset();
	virtual bool IsReady();
	
public:
	std::deque<std::shared_ptr<Actor>>* _actors;
	std::deque<std::shared_ptr<BattleAnimation>>* _anims;
	std::set<int> _input;
	std::string _name;
	bool _done;
	int _cooldown;
	int _currentCooldown;
	int _minTargets;


protected:
	void DefaultSetup();
};

#endif // !SKILL_H__
