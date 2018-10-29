#ifndef BATTLE_MANAGER_H__
#define BATTLE_MANAGER_H__

#include "fighter.h"
#include "battleUnit.h"

#include <deque>
#include <vector>
#include <map>
#include <memory>

enum PostBattleState { PBS_FightingInProgress, PBS_FightingDone, PBS_ExpAnimation, PBS_ConfirmCompletion, PBS_PostBattleComplete };

class BattleAnimationManager;

class BattleManager
{
public:
	BattleManager();
	BattleManager(std::vector<Fighter_ptr> actors);
	~BattleManager();
	void SetupHUD();
	void Update();
	int FindWinner();
	bool Animating();
	std::shared_ptr<BattleAnimationManager> GetGraphics() { return m_graphics; }

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
	Damage HandleDamage(int target);
	Damage ApplyBonusEffect(Fighter_ptr target);
	bool ValidateTargets(Fighter_ptr f);
	void ProcessSkill();
	void ProcessSkillReactions(SkillProgress prog);
	bool status_condition_met(StatusEffect status, Fighter_ptr fighter);
	std::set<int> DefaultTargetActorIndex();

	// Graphics
	void add_animation(triple& x);
	void UpdateColors();
	void SetSkillArguments(triple& x);
	void SetSkillArguments(triple& x, int applier);

	void UpdateSkillDisplay();
	void TurnStart();
	void InitiateChooseActor();
	void ResetPartyPredictedSkills();
	void SelectAction();
	void ActionProgress();
	void ActionDone();
	void TurnEnd();

	// Logic Methods
	// Method just for show, handled in input
	void SelectActor() {}
	// Method just for show, handled in input
	void SelectTargets() {}

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
	std::map<SkillProgress, std::vector<triple>> _action_buffer;


	Skill_ptr _selectedSkill;
	Fighter_ptr _owner;
	std::set<int> _selectedIndices;
	BattleState _state;
	PostBattleState _postBattleState;
	bool m_animating;
	bool _showingSkills;
	int _winner;

	int m_attackSequenceProgress;

	int _numAllies;

	int counter;

private:
	std::shared_ptr<BattleAnimationManager> m_graphics;
};

#endif
