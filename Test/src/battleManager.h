#ifndef BATTLE_MANAGER_H__
#define BATTLE_MANAGER_H__

#include <deque>
#include <vector>
#include <map>
#include <memory>
#include "actor.h"
#include "skill.h"
#include "fontManager.h"
#include "battleHUD.h"

//enum BattleState { BS_Animation, BS_Attacking, BS_SelectAction, BS_SelectTargets, BS_ActionProgress, BS_ActionDone };
const int MAX_FIGHTERS_PER_SIDE = 4;

class BattleManager
{
public:
	BattleManager();
	BattleManager(std::vector<Actor_ptr> actors);
	void Update();
	int FindWinner();
	void SetRender();

	static int DefaultTargetActorIndex(std::vector<Actor_ptr>* actors, Actor_ptr _owner, Skill_ptr selectedSkill);

private:
	void Init();
	void ManageInput();
	void UpdateLogic();
	void CycleActors();
	void UseSkill();
	void Select(int target);
	void RemoveChooseSkillText();
	void SetChooseSkillText();
	void MoveToLight(bool moveUp, bool turnEnd = false);

	void UpdateSkillDisplay();
	void TurnStart();
	void SelectAction();
	void SelectTargets();
	void ActionProgress();
	void ActionDone();
	void TurnEnd();

	void PrintAttackPrediction(Actor* actor);

public:
	std::deque<Actor_ptr> _actorQueue;
	std::vector<Actor_ptr> _actors;
	std::vector<Actor_ptr> _targets;
	std::deque<Anim_ptr> _animations;
	std::vector<Skill_ptr>* _chooseSkill;
	std::vector<unsigned int> _fonts;

	BattleHUD _hud;
	Skill_ptr _selectedSkill;
	Actor_ptr _owner;
	int _selectedIndex;
	BattleState _state;
	bool m_animating;
	bool _battleDone;
	bool _postBattleDone;
	bool _showingSkills;
	int _winner;
	
	int counter;
};

#endif
