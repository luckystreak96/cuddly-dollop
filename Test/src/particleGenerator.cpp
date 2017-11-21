#include "particleGenerator.h"

ParticleGenerator::ParticleGenerator() : m_mesh(Mesh())
{

}

Particle::Particle() : physics(PhysicsComponent(Vector3f(0, 0, 0.6f), "CENTERED_TILE")), texture("res/sprites/default.png")
{
}

void Snow::Update(Vector3f& zoneSize)
{
	velocity.x = sin(counter * 3.0f) / 20.0f;
	position += velocity;
	if (position.y < -1 || position.x < -2 || position.x > zoneSize.x + 2)
		ResetLocation(zoneSize);
	counter += 0.01f;
};

Snow::Snow(Vector3f zoneSize)
{
	texture = "res/sprites/snowflake2.png";
	ResetLocation(zoneSize, true);
}


void Snow::ResetLocation(Vector3f& zoneSize, bool firstSpawn)
{
	if (zoneSize.x == 0 || zoneSize.y == 0)
		return;
	if (firstSpawn)
		counter = fmod(((float)rand() / 100.0f), 1.0f);
	position.x = fmod(((float)rand() / 10.0f), zoneSize.x + 2.0f) - 2.0f;
	position.y = rand() % ((firstSpawn ? (int)zoneSize.y : 5)) + (firstSpawn ? 0 : zoneSize.y);
	velocity.y = -fmod(((float)rand() / 1000.0f), 0.05f) - 0.05f;
	float value = fmod(((float)rand() / 1000.0f), 0.1f);
	velocity.x = (rand() % 2) == 0 ? value : -value;
}

void Snow::SetTrans(Transformation& trans)
{
	trans.SetRotation(0, 0, counter * 10);
	trans.SetScale(Vector3f(0.3f, 0.3f, 0.3f));
}

void Rain::Update(Vector3f& zoneSize)
{
	position += velocity;
	if (/*rand() % 20 == 0 || */position.y < -1.0f || position.x < -4.0f || position.x > zoneSize.x + 1)
		ResetLocation(zoneSize);
	counter -= 0.01f;
};

Rain::Rain(Vector3f& zoneSize, std::string tex)
{
	texture = tex;
	ResetLocation(zoneSize, true);
	velocity.x = fmod((float)rand() / 10, 0.03f) + 0.04f;
}

void Rain::SetTrans(Transformation& trans)
{
	if (texture == "res/sprites/rain.png")
	{
		trans.SetRotation(0, 0, velocity.x);
		trans.SetScale(Vector3f(0.12f, 0.6f, 1.0f));
	}
	else
	{
		trans.SetRotation(0, 0, counter * 15.0f);
		trans.SetScale(Vector3f(0.5f, 0.5f, 1.0f));
	}
}


void Rain::ResetLocation(Vector3f& zoneSize, bool firstSpawn)
{
	counter = fmod(((float)rand() / 100.0f), 1.0f);
	position.x = fmod(((float)rand() / 10.0f), zoneSize.x + 4.0f) - 4.0f;
	position.y = rand() % ((firstSpawn ? (int)zoneSize.y : 10)) + (firstSpawn ? 0 : zoneSize.y);
	velocity.y = -fmod(((float)rand() / 1000.0f), 0.1f);
	velocity.y -= texture == "res/sprites/rain.png" ? 0.4f : 0.1f;
	float value = fmod(((float)rand() / 1000.0f), 0.1f);
}

void ParticleGenerator::Init(ParticleType c, unsigned int num_particles, Vector3f zoneSize, std::string tex)
{
	m_zoneSize = zoneSize;

	// Create all particles
	switch (c)
	{
	case PT_Snow:
		for (unsigned int i = 0; i < num_particles; i++)
			m_particles.push_back(std::shared_ptr<Particle>(new Snow(zoneSize)));
		break;
	case PT_Rain:
		for (unsigned int i = 0; i < num_particles; i++)
			m_particles.push_back(std::shared_ptr<Particle>(new Rain(zoneSize)));
		break;
	case PT_ObjectRain:
		for (unsigned int i = 0; i < num_particles; i++)
			m_particles.push_back(std::shared_ptr<Particle>(new Rain(zoneSize, tex)));
		break;
	default:
		for (unsigned int i = 0; i < num_particles; i++)
			m_particles.push_back(std::shared_ptr<Particle>(new Rain(zoneSize)));
	}

	FinalizeSetup();
}

void ParticleGenerator::FinalizeSetup()
{
	std::sort(m_particles.begin(), m_particles.end(), ParticleSort);
	std::sort(m_particles.begin(), m_particles.end(), ParticleSort);

	SetupMesh();
	for (auto x : m_particles)
		x->Update(m_zoneSize);

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
		d->Update(m_zoneSize);

	//INEFFICIENT BECAUSE THE MATRIX NEEDS TO BE SENT 4 TIMES - 1 PER VERTEX
	//THE PURPOSE OF THIS IS SO THAT THE VERTEX SHADER CAN KNOW THE Y COORD OF THE BITCH AND CHANGE
	//	THE Z ACCORDINGLY. THIS IS DONE THIS WAY BECAUSE OF MESHES, THEY CALCULATE POS AHEAD OF
	//	TIME AND CHANGE THE VERTEX POSITIONS -- BUT NOT ANY OTHER OBJECT. TO KEEP THE Z CONSISTENT,
	//	I USE THE MATRIX INSTEAD OF MULTIPLYING THE OTHER OBJECTS VERTEX POS BY THEIR POS.

	m_graphics->GetMModels().clear();

	Transformation t;
	std::vector<Mat4f>& mmodels = m_graphics->GetMModels();
	for (auto x : m_particles)
	{
		t.SetTranslation(x->position);
		x->SetTrans(t);
		mmodels.insert(mmodels.end(), 4, t.GetWorldTrans());
	}
}

void ParticleGenerator::SetRender()
{
	Renderer::GetInstance().Add(m_graphics);
}

void ParticleGenerator::Draw()
{
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
	return m_zoneSize;
}
