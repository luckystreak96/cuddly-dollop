#include "skillBloodyMess.h"
#include "soundManager.h"

SkillBloodyMess::SkillBloodyMess() : SkillRanged()
{
	DefaultSetup();
}

void SkillBloodyMess::DefaultSetup()
{
	//SkillRanged::DefaultSetup();
	_name = "Bloody Mess";
	_targetMode = TM_Alive;
	_targetAmount = TA_Party;
	_defaultTarget = DT_Enemy;
	_ac._start = 0.15;
	_ac._startHard = 0.35;
	_ac._end = 0.5;
	_ac._animProg = 2;
}

Damage SkillBloodyMess::CalculateDamage(StatUser& user)
{
	// Crit chance
	int roll = rand() % 100;
	if (roll <= user.Crit.Modified * 2)
		_critting = true;
	else
		_critting = false;

	// Damage
	int dmg = 4 + user.Strength.Modified * 1.6f + user.GetLevel();
	dmg += rand() % (user.GetLevel() + 2);
	if (_critting)
		dmg *= 2;

	Damage result;
	result._value = dmg;
	result._type = ST_Magical;
	result._critting = _critting;

	return result;
}

void SkillBloodyMess::ModifyAnimations()
{
	switch (m_progress)
	{
	case 3:
		std::get<2>(m_animationBuffer[3])[0] = 0.25f;// modify wait
		m_animationBuffer.insert(m_animationBuffer.begin() + 1, triple(AO_DamageParticle, AARG_Float, floats({1})));
		m_animationBuffer.insert(m_animationBuffer.end(), triple(AO_DamageParticle, AARG_Float, floats({0})));
		//std::get<1>(m_animationBuffer[1]) = AARG_FloatTargets;// modify aarg
		//std::get<2>(m_animationBuffer[1]).push_back(PT_Explosion);// modify aarg
		break;
	default:
		break;
	}
}
//
//void SkillBloodyMess::ApplyEffect()
//{
//	Damage dmg = HandleDamage(_targetProgress);
//
//	// Set wait animation to space out the attacks to better follow whats going on
//	Anim_ptr wait = Anim_ptr(new AnimWait(0.3));
//	_anims->push_back(wait);
//	_anims->push_front(Anim_ptr(new AnimColorFlash(Vector3f(3.0, 0.15, 0.15), _targets[_targetProgress])));
//
//	// Damage text
//	SpawnDamageText(_targets.at(_targetProgress), dmg);
//
//	Particle_ptr particles = Particle_ptr(new ParticleGenerator());
//	Vector3f pos = _targets.at(_targetProgress)->_Graphics->GetPos() + Vector3f(0.5f, 0.5f, 0.6f);
//	particles->SetPowerLevel(0.3f);
//	particles->Init(PT_Explosion, 15, pos, false, "blood.png");
//	//Vector3f color = _critting ? Vector3f(0.35f, 0.31f, 0.87f) : Vector3f(1.0f, 0.2f, 0.2f);
//	//particles->SetColor(color);
//	ParticleManager::GetInstance().AddParticles(particles);
//
//	SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");
//
//	_targetProgress++;
//
//	// Removes vision for next turn
//	if (_targetProgress == 1)// we only want to increment this once, not 4 times
//		_owner.lock()->_Fighter->NoPredictCountDown++;
//}
