#include "skillHeal.h"
#include "soundManager.h"

SkillHeal::SkillHeal()
{
	Skill::DefaultSetup();
	DefaultSetup();
}

void SkillHeal::DefaultSetup()
{
	_name = "Heal";
}

void SkillHeal::ApplyEffect()
{
	int dmg = 0;
	dmg -= rand() % 7 + 2;
	_targets->at(0)->TakeDamage(dmg);

	// Damage text
	SpawnDamageText(_targets->at(0), dmg);

	SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");
}
