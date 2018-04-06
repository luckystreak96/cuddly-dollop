#include "skillHeal.h"
#include "soundManager.h"
#include "gameData.h"

SkillHeal::SkillHeal()
{
	//Skill::DefaultSetup();
	DefaultSetup();
}

void SkillHeal::DefaultSetup()
{
	_name = "Heal";
	_skillType = ST_Healing;
	_targetMode = TM_Any;
	_defaultTarget = DT_Self;
}

Damage SkillHeal::CalculateDamage()
{
	Damage result;
	result._value = 3 + _owner->_Fighter->Strength.Modified / 2 + rand() % ((_owner->_Fighter->GetLevel() + 2) / 2 + 1);
	return result;
}

void SkillHeal::ApplyEffect()
{
	Damage dmg = HandleDamage();

	// Damage text
	SpawnDamageText(_targets->at(0), dmg._value);

	SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");
}
