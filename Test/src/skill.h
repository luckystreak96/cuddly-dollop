#ifndef SKILL_H__
#define SKILL_H__

#include <vector>
#include <deque>
#include <set>
#include <tuple>
#include <memory>
#include <iostream>
#include "statusEffect.h"
#include "statUser.h"

enum BattleState { BS_ChooseActor, BS_TurnStart, BS_SelectAction, BS_SelectTargets, BS_ActionProgress, BS_ActionDone, BS_TurnEnd };

class Skill;
typedef std::shared_ptr<Skill> Skill_ptr;

enum ActionCommandType { ACT_Defend, ACT_Special };
enum SkillType { ST_Physical, ST_Magical, ST_Healing, ST_Bonus };
enum TargetMode { TM_Enemy, TM_Ally, TM_Alive, TM_Dead, TM_Any };
enum DefaultTarget { DT_Self, DT_Enemy, DT_Ally };
enum TargetAmount { TA_One, TA_Party };

enum AnimationOperation {
	AS_JumpTo, AS_JumpBack, AS_ColorFlash, AS_ScreenShake, AS_BonusEffect, AS_MoveTo, AS_Wait, AS_Animation, AS_FloatingText,
	AC_CameraFollow, AC_CameraScale, AC_CameraCenter,
	AA_Start, AA_DealDamage
}; // AA_Start is there to be able to do > on the enum

enum AnimationArgument {
	AARG_Owner, AARG_Targets, AARG_Target, AARG_Float, AARG_AsyncStart, AARG_FloatAsync
};

//using namespace std;
using floats = std::vector<float>;
using triple = std::tuple<AnimationOperation, AnimationArgument, floats>;

enum SkillProgress {
	SP_0_None, SP_1_Before_Anim, SP_2_BeginAnim, SP_3_DealDamage, SP_4_PostSkillAnim, SP_5_SkillDone
};

struct Damage
{
	int _value;
	bool _critting;
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
	virtual void Setup();
	virtual SkillProgress Update();
	// Returns and then empties animations
	virtual std::vector<triple> GetAnimations();
	virtual void Reset();
	virtual bool IsReady();

	virtual Damage CalculateDamage(StatUser& user) { return Damage(); }

	// Make sure all targets are still valid
	//virtual bool ValidateTargets();

	// This method should change values to apply a skill upgrade.
	// Call Skill::ApplySkillUpgrade() at the end of the redefined method
	// returns true if an upgrade was applied
	virtual bool ApplySkillUpgrade() { _skillUpgradeProgress += 1; return true; };

	void CheckActionCommand(double percentProgress);
	void HandleActionCommand(double percentProgress);
	int action_command_level(double percentProgress);
	//bool AnimationsDone();

	//virtual void SetupProtector();

	const std::string GetName() const { return _name; }

public:
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
	TargetMode _targetMode;
	TargetAmount _targetAmount;
	DefaultTarget _defaultTarget;
	Damage _preCalculatedDamage;

protected:
	SkillProgress m_state;
	int m_progress;
	std::vector<triple> m_animationBuffer;

protected:
	virtual void DefaultSetup();
	virtual void SetAnimations();
};


#endif // !SKILL_H__
