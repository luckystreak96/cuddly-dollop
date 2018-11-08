#ifndef BATTLE_MANAGER_H__
#define BATTLE_MANAGER_H__

#include "fighter.h"
#include "battleUnit.h"
#include "battle_info.h"

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
	void Init(std::vector<Fighter_ptr> actors);
	void UpdateLogic();
	void UseSkill();
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

	// printing logic
	void print_attack_prediction(Fighter_ptr fighter);

public:
	PostBattleState _postBattleState;

private:
	std::shared_ptr<BattleAnimationManager> m_graphics;
	battle_info m_info;
	bool m_showingSkills;
};

#endif
