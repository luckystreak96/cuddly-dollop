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

int SkillHeal::CalculateDamage()
{
	int result = 5 + _owner->Strength / 2;
	return result;
}

void SkillHeal::ApplyEffect()
{
	int dmg = HandleDamage();

	// Damage text
	SpawnDamageText(_targets->at(0), dmg);

	SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");
}
