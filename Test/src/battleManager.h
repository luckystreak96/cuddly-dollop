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
	BattleManager(std::vector<Actor_ptr> actors);
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
	std::deque<Actor_ptr> _actorQueue;
	std::vector<Actor_ptr> _actors;
	std::vector<Actor_ptr> _targets;
	std::deque<Anim_ptr> _animations;
	std::vector<std::shared_ptr<Skill>>* _chooseSkill;
	std::shared_ptr<Skill> _selectedSkill;
	Actor_ptr _owner;
	int _selectedIndex;
	BattleState _state;
	bool m_animating;
	bool _done;
	
	int counter;
};

#endif