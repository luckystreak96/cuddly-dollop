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

struct Particle
{
	Particle(Vector3f mapSize);

	std::string texture;
	PhysicsComponent physics;
	Vector3f position;
	Vector3f velocity;
	float counter;
	virtual void Update(Vector3f mapSize);
	virtual void ResetLocation(Vector3f mapSize);
};

struct Snow : public Particle
{

};

class ParticleGenerator
{
public:
	ParticleGenerator(unsigned int num_particles = 1, Vector3f mapSize = Vector3f(32, 16, 0));
	~ParticleGenerator();
	void FinalizeSetup();
	void SetRender();
	void Draw();
	void Update();
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
	Vector3f m_mapSize;
};

#endif
