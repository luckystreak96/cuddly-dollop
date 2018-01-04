#ifndef BATTLE_MANAGER_H__
#define BATTLE_MANAGER_H__

#include <deque>
#include <vector>
#include <map>
#include <memory>
#include "actor.h"
#include "skill.h"

enum BattleState { BS_Animation, BS_Attacking, BS_SelectAction, BS_SelectTargets, BS_ActionProgress, BS_ActionDone };

class BattleManager
{
public:
	BattleManager();
	BattleManager(std::vector<std::shared_ptr<Actor>> actors);
	void Update();
	int FindWinner();

private:
	void Init();
	void ManageInput();
	void UpdateLogic();
	void CycleActors();
	void UseSkill();
	void Select(int target);

public:
	std::deque<std::shared_ptr<Actor>> _actorQueue;
	std::vector<std::shared_ptr<Actor>> _actors;
	std::vector<std::shared_ptr<Actor>> _targets;
	std::deque<std::shared_ptr<BattleAnimation>> _animations;
	std::vector<std::shared_ptr<Skill>>* _chooseSkill;
	std::shared_ptr<Skill> _selectedSkill;
	std::shared_ptr<Actor> _owner;
	int _selectedIndex;
	BattleState _state;
	bool m_animating;
	
	int counter;
};

#endif