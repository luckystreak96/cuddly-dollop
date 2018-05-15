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

enum PostBattleState { PBS_FightingInProgress, PBS_FightingDone, PBS_ExpAnimation, PBS_ConfirmCompletion, PBS_PostBattleComplete };

class BattleManager
{
public:
	BattleManager();
	BattleManager(std::vector<Actor_ptr> actors);
	void Update();
	int FindWinner();
	void SetRender();

	static std::vector<int> DefaultTargetActorIndex(std::vector<Actor_ptr>* actors, Actor_ptr _owner, Skill_ptr selectedSkill);

private:
	void Init();
	void UpdateLogic();
	void CycleActors();
	void UseSkill();
	void Select(int target);
	void Select(std::vector<int> targets);
	void RemoveChooseSkillText();
	void SetChooseSkillText();
	void MoveToLight(bool moveUp, bool turnEnd = false);

	void UpdateSkillDisplay();
	void TurnStart();
	void SelectActor();
	void InitiateChooseActor();
	void ResetPartyPredictedSkills();
	void SelectAction();
	void SelectTargets();
	void ActionProgress();
	void ActionDone();
	void TurnEnd();
	void BeginAnimations();

	void ManageInput();
	void HandleUpDownInput(std::set<int> input);
	void HandleLeftRightInput(std::set<int> input);
	void HandleAcceptInput();
	void HandleCancelInput();

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
	PostBattleState _postBattleState;
	bool m_isPlayerTurn;
	bool m_animating;
	bool _showingSkills;
	int _winner;

	int m_attackSequenceProgress;

	int _numAllies;

	int counter;
};

#endif
