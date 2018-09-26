#include "skillHeal.h"
#include "soundManager.h"
#include "gameData.h"
#include "battleData.h"

SkillHeal::SkillHeal() : SkillMelee()
{
	DefaultSetup();
}

void SkillHeal::DefaultSetup()
{
	//SkillMelee::DefaultSetup();
	_name = "Heal";
	_skillType = ST_Healing;
	_targetMode = TM_Any;
	_defaultTarget = DT_Self;
	_ac._start = 0.15;
	_ac._startHard = 0.35;
	_ac._end = 0.5;
	_ac._animProg = 2;
}

Damage SkillHeal::CalculateDamage(StatUser& user)
{
	//Damage result;
	//result._value = 3 + _owner.lock()->_Fighter->Strength.Modified / 2 + rand() % ((_owner.lock()->_Fighter->GetLevel() + 2) / 2 + 1);
	//return result;

	// Crit chance
	int roll = rand() % 100;
	if (roll <= user.Crit.Modified * 2)
		_critting = true;
	else
		_critting = false;

	// Damage
	int dmg = 6 + user.Strength.Modified * 2.0f + rand() % (user.GetLevel() + 8);
	if (_critting)
		dmg *= 1.5;

	Damage result;
	result._value = dmg;
	result._type = SkillType::ST_Healing;
	result._critting = _critting;

	return result;
}

void SkillHeal::ModifyAnimations()
{
	switch (m_progress)
	{
	case 3:
		std::get<2>(m_animationBuffer[0])[0] = 20.f;
		std::get<2>(m_animationBuffer[0])[1] = 2.f;
		break;
	default:
		break;
	}
}

//
//void SkillHeal::ApplyEffect()
//{
//	Damage dmg = HandleDamage();
//
//	// Damage text
//	SpawnDamageText(_targets.at(0), dmg);
//
//	SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");
//
//	_targets.at(0)->_Fighter->_Statuses.push_back(BattleData::StatusEffects.at(StatusList::Pragmatic));
//	_targets.at(0)->_Fighter->ReCalculateStats();
//	_targets.at(0)->_Fighter->UpdateObservers();
//}
