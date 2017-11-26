#ifndef PARTICLE_GENERATOR_H__
#define PARTICLE_GENERATOR_H__

#include <vector>
#include <memory>
#include "graphicsComponent.h"
#include "physicsComponent.h"
#include "mesh.h"
#include "resource_manager.h"
#include "renderer.h"
#include <time.h>

enum ParticleType { PT_Snow, PT_Rain, PT_ObjectRain, PT_Music };
static const std::map<std::string, ParticleType> ParticleFromString = { 
	std::make_pair("snow", PT_Snow),
	std::make_pair("rain", PT_Rain), 
	std::make_pair("object", PT_ObjectRain),
	std::make_pair("music", PT_Music)
};

struct Particle
{
	Particle();

	std::string texture;
	PhysicsComponent physics;
	Vector3f position;
	Vector3f velocity;
	float counter;
	virtual void Update(Vector3f& mapSize) {};
	virtual void ResetLocation(Vector3f& mapSize, bool firstSpawn = false) {};
	virtual void SetTrans(Transformation& trans) {};
};

struct Snow : public Particle
{
	Snow(Vector3f zoneSize);
	void Update(Vector3f& mapSize);
	void ResetLocation(Vector3f& mapSize, bool firstSpawn = false);
	void SetTrans(Transformation& trans);
};

struct Rain : public Particle
{
	Rain(Vector3f& zoneSize, std::string texture = "rain.png");
	void Update(Vector3f& mapSize);
	void ResetLocation(Vector3f& mapSize, bool firstSpawn = false);
	void SetTrans(Transformation& trans);
};

struct Music : public Particle
{
	Music(Vector3f& spawnPos, std::vector<std::string> texture = { "note_s.png", "note_d.png" });
	void Update(Vector3f& spawnPos);
	void ResetLocation(Vector3f& spawnPos, bool firstSpawn = false);
	void SetTrans(Transformation& trans);
	std::vector<std::string> textures;
};

class ParticleGenerator
{
public:
	ParticleGenerator();
	~ParticleGenerator();
	void Init(ParticleType c, unsigned int num_particles = 1, Vector3f zoneSize = Vector3f(32, 16, 0), std::string tex = "snowflake.png");
	void FinalizeSetup();
	void SetRender();
	void Draw();
	void LogicUpdate();
	void Update();
	void Update(Vector3f pos);
	std::vector<std::shared_ptr<Particle>>* Particles();
	unsigned int Size();
	void SetupMesh();
	std::shared_ptr<GraphicsComponent> Graphics() { return m_graphics; }

	// Returns the farthest reaches of the map in  x, y and z
	Vector3f GetRange();
	static inline bool ParticleSort(std::shared_ptr<Particle> i, std::shared_ptr<Particle> j) { return (i->position < j->position); }

private:
	std::vector<std::shared_ptr<Particle>> m_particles;
	Mesh m_mesh;
	std::shared_ptr<GraphicsComponent> m_graphics = NULL;
	int m_MBO_instances;
	unsigned int m_id;
	std::string m_texture;
	// The width height and depth of the map in tiles
	Vector3f m_zoneSize;
};

#endif
