#include "skillHeal.h"
#include "soundManager.h"
#include "gameData.h"
#include "battleData.h"

void SkillHeal::DefaultSetup()
{
	SkillMelee::DefaultSetup();
	_name = "Heal";
	_skillType = ST_Healing;
	_targetMode = TM_Any;
	_defaultTarget = DT_Self;
}

//Damage SkillHeal::CalculateDamage()
//{
//	Damage result;
//	result._value = 3 + _owner.lock()->_Fighter->Strength.Modified / 2 + rand() % ((_owner.lock()->_Fighter->GetLevel() + 2) / 2 + 1);
//	return result;
//}
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
