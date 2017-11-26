#ifndef EVENT_PARTICLE_H__
#define EVENT_PARTICLE_H__

#include "iEvent.h"
#include "entity.h"
#include "particleGenerator.h"

class EventParticle : public IEvent
{
public:
	EventParticle(int particleCount, ParticleType type, unsigned int entity_id);
	~EventParticle() {}
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	void ResetEvent();
	void SetMapSize(Vector3f mapsize);

private:
	//unsigned int m_target;
	bool m_firstSetup;
	ParticleGenerator m_particles;
	Vector3f m_mapSize;
	ParticleType m_particleType;
	unsigned int m_count;
};

#endif // !DIALOGUE_BOX_H__
