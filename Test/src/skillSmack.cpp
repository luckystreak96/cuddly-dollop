#include "skillSmack.h"
#include "soundManager.h"

SkillSmack::SkillSmack()
{
	Skill::DefaultSetup();
	DefaultSetup();
}

void SkillSmack::DefaultSetup()
{
	_name = "Smack";
}

void SkillSmack::ApplyEffect()
{
	int dmg = rand() % 3 + 5;
	_targets->at(0)->TakeDamage(dmg);

	// Damage text
	SpawnDamageText(_targets->at(0), dmg);

	SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");
}
