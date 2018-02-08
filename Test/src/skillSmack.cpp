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
	_ac._start = 0.15;
	_ac._startHard = 0.35;
	_ac._end = 0.5;
	_ac._animProg = 1;
}

int SkillSmack::CalculateDamage()
{
	// Crit chance
	int roll = rand() % 100;
	if (roll <= _owner->Crit)
		_critting = true;

	// Damage
	int result = 4 + _owner->Strength * 0.8f;
	if (_critting)
		result *= 1.5;
	
	return result;
}

void SkillSmack::ApplyEffect()
{
	int dmg = HandleDamage();

	// Damage text
	SpawnDamageText(_targets->at(0), dmg);

	Particle_ptr particles = Particle_ptr(new ParticleGenerator());
	Vector3f pos = _targets->at(0)->GetPos() + Vector3f(0.5f, 0.5f, 0.6f);
	particles->SetPowerLevel(0.3f);
	particles->Init(PT_Explosion, dmg, pos, false, "star.png");
	Vector3f color = _critting ? Vector3f(0.35f, 0.31f, 0.87f) : Vector3f(1.0f, 0.2f, 0.2f);
	particles->SetColor(color);
	ParticleManager::GetInstance().AddParticles(particles);

	SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");
}
