#ifndef PARTICLE_MANAGER_H__
#define PARTICLE_MANAGER_H__

#include <vector>
#include "particleGenerator.h"

class ParticleManager
{
public:
	static ParticleManager& GetInstance()
	{
		static ParticleManager particleMngr;
		return particleMngr;
	}

	void Update(double ElapsedTime);
	void AddParticles(Particle_ptr particles);
	void SetRender();

private:
	ParticleManager();
	std::vector<Particle_ptr> m_particles;
};

#endif // !FONT_MANAGER_H__
