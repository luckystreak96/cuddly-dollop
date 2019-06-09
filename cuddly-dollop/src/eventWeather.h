#ifndef EVENT_WEATHER_H__
#define EVENT_WEATHER_H__

#include "iEvent.h"
#include "entity.h"
#include "particleGenerator.h"

class EventWeather : public IEvent
{
public:
	EventWeather(int particleCount, ParticleType type, Vector3f mapsize, bool smooth, std::string sprite = "snow.png");
	~EventWeather() {}
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	std::shared_ptr<IEvent> Clone();
	void ResetEvent();
	void SetMapSize(Vector3f mapsize);

private:
	//unsigned int m_target;
	bool m_firstSetup;
	ParticleGenerator m_particles;
	Vector3f m_mapSize;
	ParticleType m_particleType;
	std::string m_sprite;
	unsigned int m_count;
	bool m_smooth;
};

#endif // !DIALOGUE_BOX_H__
