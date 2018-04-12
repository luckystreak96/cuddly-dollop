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

Damage SkillSmack::CalculateDamage()
{
	// Crit chance
	int roll = rand() % 100;
	//if (roll <= _owner->_Fighter->Crit.Modified)
		_critting = true;
	//else
		//_critting = false;

	// Damage
	int dmg = 2 + _owner->_Fighter->Strength.Modified * 0.8f + rand() % (_owner->_Fighter->GetLevel() + 2);
	if (_critting)
		dmg *= 1.5;

	Damage result;
	result._value = dmg;
	result._type = ST_Physical;
	
	return result;
}

void SkillSmack::ApplyEffect()
{
	Damage dmg = HandleDamage();

	// Damage text
	SpawnDamageText(_targets.at(0), dmg._value);

	Particle_ptr particles = Particle_ptr(new ParticleGenerator());
	Vector3f pos = _targets.at(0)->_Graphics->GetPos() + Vector3f(0.5f, 0.5f, 0.6f);
	particles->SetPowerLevel(0.3f);
	particles->Init(PT_Explosion, dmg._value, pos, false, "star.png");
	Vector3f color = _critting ? Vector3f(0.35f, 0.31f, 0.87f) : Vector3f(1.0f, 0.2f, 0.2f);
	particles->SetColor(color);
	ParticleManager::GetInstance().AddParticles(particles);

	SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");
}
