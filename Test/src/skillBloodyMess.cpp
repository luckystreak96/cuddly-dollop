#include "skillBloodyMess.h"
#include "soundManager.h"
#include "particleManager.h"
#include "gameData.h"
#include "animWait.h"

SkillBloodyMess::SkillBloodyMess()
{
	Skill::DefaultSetup();
	DefaultSetup();
}

void SkillBloodyMess::DefaultSetup()
{
	_name = "Bloody Mess";
	_targetMode = TM_Alive;
	_targetAmount = TA_Party;
	_defaultTarget = DT_Enemy;
	_ac._start = 0.15;
	_ac._startHard = 0.35;
	_ac._end = 0.5;
	_ac._animProg = 1;
}

Damage SkillBloodyMess::CalculateDamage()
{
	// Crit chance
	int roll = rand() % 100;
	if (roll <= _owner->_Fighter->Crit.Modified)
		_critting = true;

	// Damage
	int dmg = 4 + _owner->_Fighter->Strength.Modified * 1.6f + _owner->_Fighter->GetLevel();
	dmg += rand() % max((dmg / 20), 1);
	if (_critting)
		dmg *= 2;

	Damage result;
	result._value = dmg;
	result._type = ST_Magical;

	return result;
}

void SkillBloodyMess::ApplyEffect()
{
	Damage dmg = HandleDamage(_targetProgress);

	// Set wait animation to space out the attacks to better follow whats going on
	Anim_ptr wait = Anim_ptr(new AnimWait(0.3));
	_anims->push_back(wait);

	// Damage text
	SpawnDamageText(_targets.at(_targetProgress), dmg._value);

	Particle_ptr particles = Particle_ptr(new ParticleGenerator());
	Vector3f pos = _targets.at(_targetProgress)->_Graphics->GetPos() + Vector3f(0.5f, 0.5f, 0.6f);
	particles->SetPowerLevel(0.3f);
	particles->Init(PT_Explosion, dmg._value, pos, false, "star.png");
	Vector3f color = _critting ? Vector3f(0.35f, 0.31f, 0.87f) : Vector3f(1.0f, 0.2f, 0.2f);
	particles->SetColor(color);
	ParticleManager::GetInstance().AddParticles(particles);

	SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");

	_targetProgress++;

	// Removes vision for next turn
	_owner->_Fighter->NoPredictCountDown++;
}
