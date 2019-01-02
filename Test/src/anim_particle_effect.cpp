#include "anim_particle_effect.h"

#include "particleManager.h"

anim_particle_effect::anim_particle_effect(int amount, Vector3f colour, Vector3f* position, std::string sprite, Vector3f offset)
{
	_done = false;
	_async = true;

	m_amount = amount;
	m_colour = colour;
	m_position = position;
	m_offset = offset;
	m_sprite = sprite;
}

void anim_particle_effect::Update()
{
	// Dont update if done
	if (_done)
		return;

	// Particles
	Particle_ptr particles = Particle_ptr(new ParticleGenerator());
	particles->SetPowerLevel(0.6f);
	particles->Init(PT_Explosion, m_amount, *m_position + m_offset, false, m_sprite, Vector3f(0.5f));
	particles->SetColor(m_colour);
	ParticleManager::GetInstance().AddParticles(particles);

	_done = true;
}
