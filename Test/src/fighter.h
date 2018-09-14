#ifndef FIGHTER_H__
#define FIGHTER_H__

#include <string>
#include <memory>
#include <vector>
#include "statUser.h"
#include "passiveSkill.h"
#include "statusEffect.h"
#include "skill.h"

class Fighter;
typedef std::shared_ptr<Fighter> Fighter_ptr;

struct Damage;

class Fighter : public StatUser
{
public:
	Fighter();
	Fighter(Fighter& f);

	// Damage related
	virtual Damage TakeDamage(Damage& dmg);
	virtual Damage ApplyHealing(Damage& heal);
	virtual void ApplyBonusDamage(Fighter_ptr target);
	virtual void DamageModifiers(Damage& dmg, bool critting);
	virtual void ApplyLethal();
	void SetOrderPosition(int pos);

	// Type related
	// Returns amount of stacks of an element that is held
	int HasElement(SkillElement element);

	//Skills
	void SetSkills(std::vector<Skill_ptr>);
	const std::vector<Skill_ptr>& GetSkills() const;
	virtual void UseSkill();

	// Turn and targets
	virtual bool RespectsTargeting(Fighter_ptr owner, int targetMode);
	virtual void TurnStart(std::vector<Fighter_ptr>& actors);
	std::vector<int> GetTargets() { return _targets; }

	// Returns whether a skill could be chosen or not
	virtual bool PredictNextSkill(Fighter_ptr owner, std::vector<Fighter_ptr>* actors);

	// Action Commands
	virtual void SpecialActionCommand(Damage& dmg);
	virtual void PhysicalOffenseActionCommand(Damage& dmg);
	virtual void PhysicalDefenseActionCommand(Damage& dmg);
	virtual void MagicalOffenseActionCommand(Damage& dmg);
	virtual void MagicalDefenseActionCommand(Damage& dmg);

	// Levels
	virtual void LevelUp();
	virtual void SetLevel(int level);
	virtual void GiveExp(int xp);
	virtual void SetExp(int xp);

	// Stats
	void ReCalculateStats();
	void SetStatsFromCurve();

	// Other
	int GetId() { return m_id; }
	std::string GetSprite() { return m_sprite; }
	void SetSprite(std::string sprite) { m_sprite = sprite; }
	std::string GetName() { return m_name; }
	void SetName(std::string sprite) { m_name = sprite; }

	static bool FighterSpeedSort(Fighter_ptr a, Fighter_ptr b);
	static bool FighterBattleOrderSort(Fighter_ptr a, Fighter_ptr b);

	bool operator<(Fighter other) { _BattleFieldPosition < other._BattleFieldPosition; }

public:
	virtual int DefenseDamageModification(bool critting);

public:
	// Permanent
	std::vector<Passive_ptr> _Passives;

	// Expire after a time or a battle
	std::vector<StatusEffect> _Statuses;

	bool Dead;
	int NoPredictCountDown;
	// Flag for possible invisibility or untargetability
	bool Targetable;
	int _OrderPosition;
	int _BattleFieldPosition;
	Fighter_ptr Protector;
	int Team;
	Skill_ptr PredictedSkill;

protected:
	int m_id;
	std::string m_name;
	std::vector<Skill_ptr> m_skills;
	std::vector<int> _targets;
	std::string m_sprite;

protected:
	virtual void SetDefault();
};


#endif