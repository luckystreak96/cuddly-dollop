#include "skillProtect.h"
#include "soundManager.h"
#include "particleManager.h"
#include "gameData.h"

SkillProtect::SkillProtect()
{
	DefaultSetup();
}


void SkillProtect::DefaultSetup()
{
	_name = "Protect";
	_targetMode = TM_Ally;
	_defaultTarget = DT_Ally;
	m_statuses.push_back(Status::Status_Protected);
}

Damage SkillProtect::CalculateDamage(StatUser& user)
{
	Damage result;
	result._value = 0;
	result._type = SkillType::ST_Healing;
	result._critting = false;

	return result;
}

//void SkillProtect::ApplyEffect()
//{
//	//SpawnStatusText(_targets.at(0), "Protecting");
//	//_targets.at(0)->_Fighter->Protector = _owner.lock();
//
//	//Particle_ptr particles = Particle_ptr(new ParticleGenerator());
//	//Vector3f pos = _targets.at(0)->get_position() + Vector3f(0.5f, 0.5f, 0.6f);
//	//particles->SetPowerLevel(0.3f);
//	//particles->Init(PT_Explosion, dmg, pos, false, "star.png");
//	//particles->SetColor(Vector3f(1.f, 0.2f, 0.2f));
//	//ParticleManager::GetInstance().AddParticles(particles);
//
//	//SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");
//}
