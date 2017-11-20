#include "particleGenerator.h"

ParticleGenerator::ParticleGenerator(unsigned int numParticles, Vector3f mapSize) : m_mesh(Mesh()), m_mapSize(mapSize)
{
	// Create all particles
	for (unsigned int i = 0; i < numParticles; i++)
	{
		m_particles.push_back(std::shared_ptr<Particle>(new Particle(mapSize)));
	}

	FinalizeSetup();
}

void Particle::Update(Vector3f mapSize)
{
	velocity.x = sin(counter * 3.0f) / 20.0f;
	position += velocity;
	if (position.y < -1 || position.x < -1 || position.x > mapSize.x + 1)
		ResetLocation(mapSize);
	counter += 0.01f;
};

Particle::Particle(Vector3f mapSize) : physics(PhysicsComponent(Vector3f(), "CENTERED_TILE")), texture("res/sprites/snowflake2.png")
{
	ResetLocation(mapSize);
}

void Particle::ResetLocation(Vector3f mapSize)
{
	counter = fmod(((float)rand() / 100.0f), 1.0f);
	position.x = fmod(((float)rand() / 10.0f), mapSize.x);
	position.y = rand() % 5 + mapSize.y;
	velocity.y = -fmod(((float)rand() / 1000.0f), 0.05f);
	velocity.y -= 0.05f;
	float value = fmod(((float)rand() / 1000.0f), 0.1f);
	velocity.x = (rand() % 2) == 0 ? value : -value;
}

void ParticleGenerator::FinalizeSetup()
{
	std::sort(m_particles.begin(), m_particles.end(), ParticleSort);
	std::sort(m_particles.begin(), m_particles.end(), ParticleSort);

	SetupMesh();
	for (auto x : m_particles)
		x->Update(m_mapSize);

	Update();
}

ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::SetupMesh()
{
	m_mesh.Reset();
	std::sort(m_particles.begin(), m_particles.end(), ParticleSort);
	for (auto t : m_particles)
	{
		m_mesh.AddToMesh(t->physics.GetVertices(), t->physics.GetIndices(), t->physics.GetHighestIndex(), t->position, t->texture, -1);
	}

	m_MBO_instances = m_particles.size();

	m_texture = "res/tiles.png";
	//m_mesh.Finalize(m_texture);
	m_graphics = std::shared_ptr<GraphicsComponent>(new GraphicsComponent(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices(), m_texture));
}

void ParticleGenerator::Update()
{
	for (auto d : m_particles)
		d->Update(m_mapSize);

	//SetupMesh();

	//Drawable::Update();

	//INEFFICIENT BECAUSE THE MATRIX NEEDS TO BE SENT 4 TIMES - 1 PER VERTEX
	//THE PURPOSE OF THIS IS SO THAT THE VERTEX SHADER CAN KNOW THE Y COORD OF THE BITCH AND CHANGE
	//	THE Z ACCORDINGLY. THIS IS DONE THIS WAY BECAUSE OF MESHES, THEY CALCULATE POS AHEAD OF
	//	TIME AND CHANGE THE VERTEX POSITIONS -- BUT NOT ANY OTHER OBJECT. TO KEEP THE Z CONSISTENT,
	//	I USE THE MATRIX INSTEAD OF MULTIPLYING THE OTHER OBJECTS VERTEX POS BY THEIR POS.
	if (m_graphics->GetModels()->size() == 0)
	{
		//m_graphics->GetModels() = new std::vector<Vector3f>();

		for (auto x : m_particles)
		{
			Vector3f pos = x->physics.Position();
			m_graphics->GetModels()->insert(m_graphics->GetModels()->end(), 4, pos);
		}
	}

	//if (m_graphics->GetMModels()->size() == 0)
	//{
		m_graphics->GetMModels()->clear();

		for (auto x : m_particles)
		{
			Vector3f pos = x->position;
			pos.z = 0.6f;
			Transformation t;
			t.SetTranslation(pos);
			t.SetRotation(0, 0, x->counter);
			t.SetScale(Vector3f(0.3f, 0.3f, 0.3f));
			m_graphics->GetMModels()->insert(m_graphics->GetMModels()->end(), 4, t.GetWorldTrans());
		}
	//}

	//m_graphics->Update();

	//if (m_models.size() == 0)
	//{
	//	m_models = std::vector<Mat4f>();

	//	for (auto x : m_tiles)
	//		m_models.insert(m_models.end(), 4, x->GetModelMat()->GetWorldTrans());

	//	m_MBO_instances = m_models.size();
	//}
}

void ParticleGenerator::SetRender()
{
	m_graphics->GetModelMat()->SetTranslation(Vector3f(10, 10, 0));
	Renderer::GetInstance().Add(m_graphics);
}

void ParticleGenerator::Draw()
{
	//Set to 0 so the mesh doesnt move all the objects as well.
	//This should only be modified if the whole mesh is moving, 
	//otherwise each individual object inside it has its own coords set in the vertices.
	m_graphics->GetModelMat()->SetTranslation(Vector3f(10, 10, 0));
	m_graphics->Draw();
}

std::vector<std::shared_ptr<Particle>>* ParticleGenerator::Particles()
{
	return &m_particles;
}

unsigned int ParticleGenerator::Size()
{
	return (unsigned int)m_particles.size();
}

Vector3f ParticleGenerator::GetRange()
{
	return m_mapSize;
}
