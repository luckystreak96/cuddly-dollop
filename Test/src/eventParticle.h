#ifndef EVENT_PARTICLE_H__
#define EVENT_PARTICLE_H__

#include "iEvent.h"
#include "entity.h"
#include "particleGenerator.h"

class EventParticle : public IEvent
{
public:
	EventParticle(int particleCount, ParticleType type, unsigned int entity_id, std::string sprite_override = "");
	~EventParticle() {}
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	std::shared_ptr<IEvent> Clone();
	void ResetEvent();
	void SetMapSize(Vector3f mapsize);
	void SetPower(float power);

private:
	//unsigned int m_target;
	bool m_firstSetup;
	ParticleGenerator m_particles;
	Vector3f m_mapSize;
	ParticleType m_particleType;
	std::string m_sprite;
	unsigned int m_count;
	float m_power;
};

#endif // !DIALOGUE_BOX_H__
