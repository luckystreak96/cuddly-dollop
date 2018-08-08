#ifndef SKILL_H__
#define SKILL_H__

#include <vector>
#include <deque>
#include <set>
#include <memory>
#include <iostream>
#include "actor.h"
#include "statusEffect.h"
#include "battleAnimation.h"

enum BattleState { BS_ChooseActor, BS_TurnStart, BS_SelectAction, BS_SelectTargets, BS_ActionProgress, BS_ActionDone, BS_TurnEnd };

class Skill;
typedef std::shared_ptr<Skill> Skill_ptr;

enum ActionCommandType { ACT_Defend, ACT_Special };
enum SkillType { ST_Physical, ST_Magical, ST_Healing, ST_Bonus };
enum TargetMode { TM_Enemy, TM_Ally, TM_Alive, TM_Dead, TM_Any };
enum DefaultTarget { DT_Self, DT_Enemy, DT_Ally };
enum TargetAmount { TA_One, TA_Party };

struct Damage
{
	int _value;
	SkillType _type;
};

struct ActionCommand
{
	double _start;
	double _startHard;
	double _end;
	bool _success;
	bool _tried;
	int _animProg;
	ActionCommandType _type;
};

class Skill
{
public:
	Skill();
	virtual BattleState Setup(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner);
	virtual void Start();
	virtual void Update();
	virtual void Reset();
	virtual bool IsReady();
	virtual void SpawnDamageText(Actor_ptr target, Damage dmg);
	virtual void SpawnStatusText(Actor_ptr target, std::string statusName);
	virtual Damage CalculateDamage() { return Damage(); }
	virtual Damage HandleDamage(int target = 0);
	virtual void ApplyBonusEffect();

	// Make sure all targets are still valid
	virtual bool ValidateTargets();

	// This method should change values to apply a skill upgrade.
	// Call Skill::ApplySkillUpgrade() at the end of the redefined method
	// returns true if an upgrade was applied
	virtual bool ApplySkillUpgrade() { _skillUpgradeProgress += 1; return true; };

	void CheckActionCommand(double percentProgress);
	void HandleActionCommand(double percentProgress);
	bool AnimationsDone();

	virtual void SetupProtector();

public:
	std::deque<Actor_ptr>* _actors;
	std::deque<Anim_ptr>* _anims;
	std::vector<Actor_ptr> _targets;
	std::set<int> _input;
	std::string _name;
	SkillType _skillType;
	SkillElement _skillElement;
	bool _done;
	bool _critting;
	bool _isPreCalculated;
	int _animProg;
	int _cooldown;
	int _currentCooldown;
	int _minTargets;
	int _skillUpgradeProgress;
	// Maximum upgrades to the skill (so you dont waste skill points)
	int _skillUpgradeMax;
	ActionCommand _ac;
	Actor_ptr _owner;
	TargetMode _targetMode;
	TargetAmount _targetAmount;
	DefaultTarget _defaultTarget;
	Damage _preCalculatedDamage;

protected:
	void DefaultSetup();
};


#endif // !SKILL_H__
