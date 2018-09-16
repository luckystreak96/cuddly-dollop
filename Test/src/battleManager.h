#ifndef BATTLE_MANAGER_H__
#define BATTLE_MANAGER_H__

#include <deque>
#include <vector>
#include <map>
#include <memory>
#include "fighter.h"
#include "fontManager.h"
#include "battleHUD.h"

enum PostBattleState { PBS_FightingInProgress, PBS_FightingDone, PBS_ExpAnimation, PBS_ConfirmCompletion, PBS_PostBattleComplete };

class BattleAnimationManager;

class BattleManager
{
public:
	BattleManager();
	BattleManager(std::vector<Fighter_ptr> actors);
	~BattleManager();
	void Update();
	int FindWinner();
	void SetRender();
	bool Animating();

	static std::set<int> DefaultTargetActorIndex(std::vector<Fighter_ptr>* actors, Fighter_ptr _owner, Skill_ptr selectedSkill);

private:
	void Init();
	void UpdateLogic();
	void CycleActors();
	void UseSkill();
	void Select(int target);
	void Select(std::set<int> targets);
	void RemoveChooseSkillText();
	void SetChooseSkillText();
	void MoveToLight(bool moveUp, bool turnEnd = false);
	
	// Skills
	Damage HandleDamage(Skill_ptr skill, int target);
	void ApplyBonusEffect(Fighter_ptr target);

	// Graphics
	void UpdateColors();
	void ExpAnimation(Fighter_ptr, int xp);
	void ProcessSkill();
	void SetSkillArguments(triple& x);

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

	void PrintAttackPrediction(Fighter_ptr actor);

public:
	std::deque<Fighter_ptr> _actorQueue;
	std::vector<Fighter_ptr> _actors;
	std::set<int> _targets;
	std::vector<Skill_ptr> _chooseSkill;
	std::vector<unsigned int> _fonts;


	BattleHUD _hud;
	Skill_ptr _selectedSkill;
	Fighter_ptr _owner;
	std::set<int> _selectedIndices;
	BattleState _state;
	PostBattleState _postBattleState;
	bool m_isPlayerTurn;
	bool m_animating;
	bool m_singleFileAttacks;
	bool _showingSkills;
	int _winner;

	int m_attackSequenceProgress;

	int _numAllies;

	int counter;

private:
	std::unique_ptr<BattleAnimationManager> m_graphics;
};

#endif
