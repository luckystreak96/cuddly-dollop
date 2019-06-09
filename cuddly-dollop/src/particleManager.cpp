#include "particleManager.h"

ParticleManager::ParticleManager()
{
}

void ParticleManager::Update(double ElapsedTime)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		m_particles[i]->Update();
		if (m_particles[i]->completed)
		{
			m_particles.erase(m_particles.begin() + i);
			i--;
		}
	}
}

void ParticleManager::AddParticles(Particle_ptr particles)
{
	m_particles.push_back(particles);
}

void ParticleManager::SetRender()
{
	for (auto& x : m_particles)
	{
		x->SetRender();
	}
}

