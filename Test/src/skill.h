#ifndef SKILL_H__
#define SKILL_H__

#include <vector>
#include <deque>
#include <set>
#include <memory>
#include <iostream>
#include "actor.h"
#include "battleAnimation.h"

enum BattleState { BS_TurnStart, BS_SelectAction, BS_SelectTargets, BS_ActionProgress, BS_ActionDone, BS_TurnEnd };

class Skill;
typedef std::shared_ptr<Skill> Skill_ptr;

enum ActionCommandType { ACT_Defend, ACT_Special };
enum SkillType { ST_Physical, ST_Magical, ST_Healing };
enum TargetMode { TM_Enemy, TM_Ally, TM_Alive, TM_Dead, TM_Any };
enum DefaultTarget { DT_Self, DT_Enemy, DT_Ally };

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
	virtual BattleState Start(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner);
	virtual void Update();
	virtual void Reset();
	virtual bool IsReady();
	virtual void SpawnDamageText(Actor_ptr target, int dmg);
	virtual void SpawnStatusText(Actor_ptr target, std::string statusName);
	virtual Damage CalculateDamage() { return Damage(); }
	virtual Damage HandleDamage();
	void CheckActionCommand(double percentProgress);
	void HandleActionCommand(double percentProgress);
	bool AnimationsDone();

public:
	std::deque<Actor_ptr>* _actors;
	std::vector<Actor_ptr>* _targets;
	std::deque<Anim_ptr>* _anims;
	std::set<int> _input;
	std::string _name;
	SkillType _skillType;
	bool _done;
	bool _critting;
	int _animProg;
	int _cooldown;
	int _currentCooldown;
	int _minTargets;
	ActionCommand _ac;
	Actor_ptr _owner;
	TargetMode _targetMode;
	DefaultTarget _defaultTarget;

protected:
	void DefaultSetup();
};


#endif // !SKILL_H__
