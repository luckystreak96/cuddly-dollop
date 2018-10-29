#include "skillSmack.h"
#include "soundManager.h"
#include "particleManager.h"
#include "gameData.h"
#include "animColorFlash.h"
#include "animScreenShake.h"
#include "battleData.h"

SkillSmack::SkillSmack() : SkillMelee()
{
	DefaultSetup();
}


void SkillSmack::DefaultSetup()
{
	//SkillMelee::DefaultSetup();
	m_statuses.push_back(Status_Determined);
	_name = "Smack";
	_skillElement = SE_Determined;
	_targetMode = TM_Alive;
	_defaultTarget = DT_Enemy;
	_ac._start = 0.15;
	_ac._startHard = 0.35;
	_ac._end = 0.5;
	_ac._animProg = 2;
}

Damage SkillSmack::CalculateDamage(StatUser& user)
{
	// Crit chance
	int roll = rand() % 100;
	if (roll <= user.Crit.Modified)
		_critting = true;
	else
		_critting = false;

	// Damage
	int dmg = 1 + user.Strength.Modified * 0.4f + rand() % (user.GetLevel() + 2);
	if (_critting)
		dmg *= 1.5;

	Damage result;
	result._value = dmg;
	result._type = ST_Physical;
	result._critting = _critting;
	
	return result;
}
//
//void SkillSmack::ApplyEffect()
//{
//	Damage dmg = HandleDamage();
//
//	// Damage text
//	SpawnDamageText(_targets.at(0), dmg);
//	_anims->push_front(Anim_ptr(new AnimColorFlash(Vector3f(3, 3, 5), _targets[0])));
//	//_anims->push_front(Anim_ptr(new AnimScreenShake()));
//
//	Particle_ptr particles = Particle_ptr(new ParticleGenerator());
//	Vector3f pos = _targets.at(0)->_Graphics->GetPos() + Vector3f(0.5f, 0.5f, 0.6f);
//	particles->SetPowerLevel(0.3f);
//	particles->Init(PT_Explosion, dmg._value, pos, false, "star.png");
//	Vector3f color = _critting ? Vector3f(0.35f, 0.31f, 0.87f) : Vector3f(1.0f, 0.2f, 0.2f);
//	particles->SetColor(color);
//	ParticleManager::GetInstance().AddParticles(particles);
//
//	SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");
//
//	_targets.at(0)->_Fighter->_Statuses.push_back(BattleData::StatusEffects.at(Status::Status_Determined));
//	_targets.at(0)->_Fighter->ReCalculateStats();
//}
