#include "particleGenerator.h"
#include "define.h"
#include <thread>

ParticleGenerator::ParticleGenerator() : m_mesh(Mesh()), m_power(2.0f), completed(false)
{

}

Particle::Particle()
	: physics(PhysicsComponent(Vector3f(0, 0, 0.6f), "CENTERED_PARTICLE_TILE")),
	texture("snowflake.png"), position(Vector3f(0, 0, 0.6f)), done(false)
{
}


//========= SNOW ============

void Snow::Update(Vector3f& zoneSize)
{
	velocity.x = sin(counter * 3.0f) / 20.0f;
	position += velocity;
	if (position.y < -1 || position.x < -2 || position.x > zoneSize.x + 2)
		ResetLocation(zoneSize);
	counter += 0.01f;
};

Snow::Snow(Vector3f zoneSize, bool smooth)
{
	texture = "snowflake.png";
	size = fmod((float)rand() / 10.f, 0.1f) + 0.2f;
	ResetLocation(zoneSize, true, smooth);
}


void Snow::ResetLocation(Vector3f& zoneSize, bool firstSpawn, bool smooth)
{
	if (zoneSize.x == 0 || zoneSize.y == 0)
		return;
	if (firstSpawn || smooth)
		counter = fmod(((float)rand() / 1000.0f), 2.0f);

	if (smooth)
		firstSpawn = false;

	position.x = fmod(((float)rand() / 1000.0f), zoneSize.x + 2.0f) - 2.0f;
	position.y = fmod(rand() / 1000.0f, ((firstSpawn ? (int)zoneSize.y * 2 : (int)zoneSize.y))) + (firstSpawn ? 0 : zoneSize.y);
	velocity.y = -fmod(((float)rand() / 1000.0f), 0.003f) - 0.003f;
	velocity.y *= pow(size * 10.f, 2);
	float value = fmod(((float)rand() / 1000.0f), 0.1f);
	velocity.x = (rand() % 2) == 0 ? value : -value;
}

void Snow::SetTrans(Transformation& trans)
{
	trans.SetRotation(0, 0, counter * 10);
	trans.SetScale(Vector3f(size, size, 0.3f));
}


//========= RAIN ============

void Rain::Update(Vector3f& zoneSize)
{
	position += velocity;
	if (/*rand() % 20 == 0 || */position.y < -1.0f || position.x < -4.0f || position.x > zoneSize.x + 1)
		ResetLocation(zoneSize);
	counter -= 0.01f;
};

Rain::Rain(Vector3f& zoneSize, std::string tex, bool smooth)
{
	texture = tex;
	ResetLocation(zoneSize, true, smooth);
	velocity.x = fmod((float)rand() / 10, 0.03f) + 0.04f;
}

void Rain::SetTrans(Transformation& trans)
{
	if (texture == "rain.png")
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


void Rain::ResetLocation(Vector3f& zoneSize, bool firstSpawn, bool smooth)
{
	if (smooth)
		firstSpawn = false;

	counter = fmod(((float)rand() / 100.0f), 1.0f);
	position.x = fmod(((float)rand() / 10.0f), zoneSize.x + 4.0f) - 4.0f;
	position.y = rand() % ((firstSpawn ? (int)zoneSize.y : 10)) + (firstSpawn ? 0 : zoneSize.y);
	velocity.y = -fmod(((float)rand() / 1000.0f), 0.3f);
	velocity.y -= texture == "rain.png" ? 0.6f : 0.1f;
	float value = fmod(((float)rand() / 1000.0f), 0.1f);
}

//========= MUSIC ============

void Music::Update(Vector3f& spawnPos)
{
	position += velocity;
	if (counter > 180)
	{
		counter = 0;
		ResetLocation(spawnPos);
	}
	counter += 1.f;
};

Music::Music(Vector3f& spawnPos, std::vector<std::string> tex, bool smooth)
{
	textures = tex;
	ResetLocation(spawnPos, true, smooth);
	velocity.x = fmod((float)rand() / 10, 0.02f) + 0.01f;
}

void Music::SetTrans(Transformation& trans)
{
	trans.SetScale(Vector3f(0.5f, 0.5f, 1.0f));
}


void Music::ResetLocation(Vector3f& spawnPos, bool firstSpawn, bool smooth)
{
	texture = textures.at(rand() % textures.size());
	if (firstSpawn)
	{
		position.x = -10;
		position.y = -10;
		counter = (float)(rand() % 180);
	}
	else
	{
		counter = (float)(rand() % 20);
		position.x = spawnPos.x;
		position.y = spawnPos.y;
		velocity.y = fmod(((float)rand() / 1000.0f), 0.02f);
	}
}

//========= EXPLOSION ============

void Explosion::Update(Vector3f& spawnpos)
{
	position += velocity;
	velocity.x *= 0.95f;
	velocity.y += 0.0005f;
	if (counter >= 60)
		done = true;//ResetLocation(spawnpos);
	else
		counter += 1.f;
};

Explosion::Explosion(Vector3f& spawnPos, std::string tex, bool smooth, float pow)
{
	if (tex == "")
		texture = "dust.png";
	else
		texture = tex;

	power = pow;
	ResetLocation(spawnPos, true, smooth);
}

void Explosion::SetTrans(Transformation& trans)
{
	if (texture == "dust.png")
		trans.SetRotation(counter / 60.f, 0, counter * (velocity.x / abs(velocity.x)) / 4.f);
	else
		trans.SetRotation(0, 0, counter * (velocity.x / abs(velocity.x)) / 8.f);
	float value = 0.1f * counter;
	float scale = (36.f - value * value) / 36.f;
	trans.SetScale(Vector3f(0.5f * scale, 0.5f * scale, 1.0f));
}


void Explosion::ResetLocation(Vector3f& spawnPos, bool firstSpawn, bool smooth)
{
	counter = rand() % 10;
	position.x = spawnPos.x;
	position.y = spawnPos.y;
	velocity.x = (fmod((float)rand() / 1000.f, power) - power / 2.f);
	// Velocity minimum clamp
	velocity.x = velocity.x < 0 ? fmin(velocity.x, -0.02f) : fmax(velocity.x, 0.02f);
	velocity.y = fmod(((float)rand() / 1000.0f), 0.06f) - 0.03f;
}

//========= PARTICLE GENERATOR ============

void ParticleGenerator::Init(ParticleType c, unsigned int num_particles, Vector3f zoneSize, bool smooth, std::string tex)
{
	m_zoneSize = zoneSize;

	// Create all particles
	switch (c)
	{
	case PT_Snow:
		for (unsigned int i = 0; i < num_particles; i++)
			m_particles.push_back(std::shared_ptr<Particle>(new Snow(zoneSize, smooth)));
		break;
	case PT_Rain:
		for (unsigned int i = 0; i < num_particles; i++)
			m_particles.push_back(std::shared_ptr<Particle>(new Rain(zoneSize)));
		break;
	case PT_ObjectRain:
		for (unsigned int i = 0; i < num_particles; i++)
			m_particles.push_back(std::shared_ptr<Particle>(new Rain(zoneSize, tex)));
		break;
	case PT_Music:
		for (unsigned int i = 0; i < num_particles; i++)
			m_particles.push_back(std::shared_ptr<Particle>(new Music(zoneSize)));
		break;
	case PT_Explosion:
		for (unsigned int i = 0; i < num_particles; i++)
			m_particles.push_back(std::shared_ptr<Particle>(new Explosion(zoneSize, tex, false, m_power)));
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
	//for (auto x : m_particles)
	//	x->Update(m_zoneSize);

	Update();
}

ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::SetupMesh()
{
	m_mesh.Reset();
	//m_mesh._instancedDraw = true;
	std::sort(m_particles.begin(), m_particles.end(), ParticleSort);
	for (auto t : m_particles)
	{
		std::vector<Vertex> verts = t->physics.GetVertices();
		m_mesh.AddToMesh(verts, t->physics.GetIndices(), t->physics.GetHighestIndex(), t->position, t->texture);
	}

	m_MBO_instances = m_particles.size();

	m_texture = "res/tiles.png";
	//m_mesh.Finalize(m_texture);
	m_graphics = GraphComp_ptr(new GraphicsComponent(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices(), m_texture));
	//m_graphics->_instancedDraw = true;
	m_graphics->SetPhysics(Vector3f(0, 0, 0.6f), Vector3f());
}

void ParticleGenerator::LogicUpdate()
{
	int count = 0;
	for (auto &d : m_particles)
	{
		d->Update(m_zoneSize);
		if (d->done)
			count++;
	}

	if (count == m_particles.size())
		completed = true;

	//INEFFICIENT BECAUSE THE MATRIX NEEDS TO BE SENT 4 TIMES - 1 PER VERTEX
	//THE PURPOSE OF THIS IS SO THAT THE VERTEX SHADER CAN KNOW THE Y COORD OF THE BITCH AND CHANGE
	//	THE Z ACCORDINGLY. THIS IS DONE THIS WAY BECAUSE OF MESHES, THEY CALCULATE POS AHEAD OF
	//	TIME AND CHANGE THE VERTEX POSITIONS -- BUT NOT ANY OTHER OBJECT. TO KEEP THE Z CONSISTENT,
	//	I USE THE MATRIX INSTEAD OF MULTIPLYING THE OTHER OBJECTS VERTEX POS BY THEIR POS.

	m_graphics->GetMModels().clear();

	Transformation t;
	for (auto &x : m_particles)
	{
		t.SetTranslation(x->position);
		x->SetTrans(t);
		m_graphics->InsertMModels(t);
	}
}

void ParticleGenerator::Update()
{
	LogicUpdate();
}

void ParticleGenerator::Update(Vector3f pos)
{
	m_zoneSize = pos;
	LogicUpdate();
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

void ParticleGenerator::SetPowerLevel(float power)
{
	m_power = power;
}

void ParticleGenerator::SetColor(Vector3f color, float alpha)
{
	m_graphics->SetColorAll(color, alpha);
}
