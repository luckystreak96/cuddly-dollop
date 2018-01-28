#include "skillSmack.h"
#include "soundManager.h"
#include "particleManager.h"
#include "gameData.h"

SkillSmack::SkillSmack()
{
	Skill::DefaultSetup();
	DefaultSetup();
}

void SkillSmack::DefaultSetup()
{
	_name = "Smack";
	_targetMode = TM_Alive;
	_defaultTarget = DT_Enemy;
}

void SkillSmack::ApplyEffect()
{
	int dmg = rand() % 3 + 5;
	_targets->at(0)->TakeDamage(dmg);

	// Damage text
	SpawnDamageText(_targets->at(0), dmg);

	Particle_ptr particles = Particle_ptr(new ParticleGenerator());
	Vector3f pos = _targets->at(0)->GetPos() + Vector3f(0.5f, 0.5f, 0.6f);
	particles->SetPowerLevel(0.3f);
	particles->Init(PT_Explosion, dmg, pos, false, "star.png");
	particles->SetColor(Vector3f(1.f, 0.2f, 0.2f));
	ParticleManager::GetInstance().AddParticles(particles);

	SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");
}
