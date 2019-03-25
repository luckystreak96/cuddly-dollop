#include "particleGenerator.h"

#include "renderer.h"
#include "graphicsComponent.h"

#include <time.h>
#include <algorithm>

#undef min
#undef max

ParticleGenerator::ParticleGenerator() : m_power(2.0f), completed(false)
{
	m_texture = "res/tiles.png";
	m_mesh.init_static_atlas(GraphComp_ptr(new GraphicsComponent("", m_texture)), "CENTERED_PARTICLE_TILE");
	m_mesh.set_graphics_position(Vector3f(0, 0, 0.6f));
}

Particle::Particle() : texture("snowflake.png"), position(Vector3f(0, 0, 0.6f)),
done(false), size(Vector3f(1, 1, 1)), color(Vector3f(1.f)), alpha(1.f)
{
}


//========= SNOW ============

void Snow::Update(Vector3f& zoneSize)
{
	velocity.x = sin(counter * 3) - 1;
	position += velocity;
	if (position.y < -1 * 64 || position.x < -2 * 64 || position.x > zoneSize.x + 2 * 64)
		ResetLocation(zoneSize);
	counter += 0.01f;
};

Snow::Snow(Vector3f zoneSize, bool smooth)
{
	texture = "snowflake.png";
	float snowSize = fmod((float)rand() / 10.f, 0.1f) + 0.2f;
	size.x = snowSize;
	size.y = snowSize;
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
	velocity.y = -(rand() % 4) - 1;
//	velocity.y *= pow(size.x * 10.f, 2);
	//velocity.x = -(rand() % 2) - 1;
}

void Snow::SetTrans(Transformation& trans)
{
	trans.SetRotation(0, 0, counter * 10);
	trans.SetScale(size);
}


//========= LEAF ============

void Leaf::Update(Vector3f& zoneSize)
{
	velocity.x = sin(counter * 3.0f) * 5.0f;
	position += velocity;
	if (position.y < -1 * 64 || position.x < -2 * 64 || position.x > zoneSize.x + 2.0f * 64.0f)
		ResetLocation(zoneSize);
	counter += 0.01f;
};

Leaf::Leaf(Vector3f zoneSize, bool smooth)
{
	texture = "leaf.png";
	size = fmod((float)rand() / 10.f, 0.1f) + 0.2f;
	ResetLocation(zoneSize, true, smooth);
}


void Leaf::ResetLocation(Vector3f& zoneSize, bool firstSpawn, bool smooth)
{
	if (zoneSize.x == 0 || zoneSize.y == 0)
		return;
	if (firstSpawn || smooth)
		counter = fmod(((float)rand() / 1000.0f), 2.0f);

	if (smooth)
		firstSpawn = false;

	//position.x = fmod(((float)rand() / 1000.0f), zoneSize.x + 2.0f) - 2.0f;
	position.x = rand() % ((int)zoneSize.x + 64) - 64;
	//std::cout << position.x << std::endl;
	position.y = fmod(rand() / 10.0f, ((firstSpawn ? (int)zoneSize.y * 2 : (int)zoneSize.y))) + (firstSpawn ? 0 : zoneSize.y);
	//velocity.y *= pow(size.x * 0.2f, 2);
	float value = rand() % 16;
	velocity.x = (rand() % 64) == 0 ? value : -value;
	velocity.y = -(rand() % 6) - 1;
	int random = rand();

	// Set color
	Vector3f base = Vector3f(0.5f, 0.5f, 0.5f);
	if (random % 5 == 0)
	{
		color = base + Vector3f(0, 0.5f, 0.1f);
	}
	else if (random % 4 == 0)
	{
		color = base + Vector3f(0, 0.9f, 0.03f);
	}
	else if (random % 3 == 0)
	{
		color = base + Vector3f(0.65f, 0.82f, 0.02f);
	}
	else if (random % 2 == 0)
	{
		color = base + Vector3f(1.0f, 0.85f, 0.02f);
	}
	else
	{
		color = base + Vector3f(0.8f, 0.8f, 0.1f);
	}
}

void Leaf::SetTrans(Transformation& trans)
{
	trans.SetRotation(0, 0, counter * 10);
	// Maybe the 0.3 z-scale was useful for something?
	trans.SetScale(size/*Vector3f(size, size, 0.3f)*/);
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
	size = Vector3f(0.5f);
	size.z = 1.0f;
	textures = tex;
	ResetLocation(spawnPos, true, smooth);
	velocity.x = (rand() % 2) / 3.0f + 0.5f;
}

void Music::SetTrans(Transformation& trans)
{
	trans.SetScale(size);
}


void Music::ResetLocation(Vector3f& spawnPos, bool firstSpawn, bool smooth)
{
	texture = textures.at(rand() % textures.size());
	if (firstSpawn)
	{
		position.x = -100;
		position.y = -100;
		counter = (float)(rand() % 180);
	}
	else
	{
		counter = (float)(rand() % 20);
		position.x = spawnPos.x;
		position.y = spawnPos.y;
		velocity.y = (rand() % 6) / 6.0f + 0.25f;
	}
}

//========= EXPLOSION ============

void Explosion::Update(Vector3f& spawnpos)
{
	position += velocity;
	velocity.x *= 0.9f;
	velocity.y += 0.05f;
	if (counter >= 60)
		done = true;//ResetLocation(spawnpos);
	else
		counter += 1.f;
};

Explosion::Explosion(Vector3f& spawnPos, std::string tex, bool smooth, float pow, Vector3f size)
{
	if (tex == "")
		texture = "dust.png";
	else
		texture = tex;

	power = pow;
	this->size = size;
	this->size.z = 1.f;
	ResetLocation(spawnPos, true, smooth);
	//size = Vector3f(0.5f, 0.5f, 1.0f);
}

void Explosion::SetTrans(Transformation& trans)
{
	if (texture == "dust.png")
		trans.SetRotation(counter / 60.f, 0, counter * (velocity.x / abs(velocity.x)) / 4.f);
	else
		trans.SetRotation(0, 0, counter * (velocity.x / abs(velocity.x)) / 8.f);
	float value = 0.1f * counter;
	float scale = (60.0f - counter) / 60.0f; // 60 is the max counter before its done
	scale *= 0.5f;
	Vector3f result(1.f);
	result.x = scale * size.x;
	result.y = scale * size.y;
	trans.SetScale(result);
}


void Explosion::ResetLocation(Vector3f& spawnPos, bool firstSpawn, bool smooth)
{
	//size = Vector3f(1.5f, 1.5f, 1.0f);
	counter = rand() % 10;
	position.x = spawnPos.x;
	position.y = spawnPos.y;
	velocity.x = fmod((float)rand() / 10.f, (power * 10.f)) - (power * 10.f) / 2.0f;
	// get_velocity minimum clamp
	velocity.x = velocity.x < 0 ? std::min(velocity.x, -0.5f) : std::max(velocity.x, 0.5f);
	velocity.y = fmod((float)rand() / 10.f, 2.0f) - 1.f;
}

//========= PARTICLE GENERATOR ============

void ParticleGenerator::Init(ParticleType c, unsigned int num_particles, Vector3f zoneSize, bool smooth, std::string tex, Vector3f size)
{
	std::cout << "Zonesize: " << zoneSize.y << std::endl;
	if (num_particles == 0)
	{
		num_particles = 1;
		c = PT_Explosion;
		tex = "dust.png";
	}
	else if (num_particles > 10000)
	{
		num_particles = 10000;
		std::cout << "Particle cap reached, setting to 10,000." << std::endl;
	}

	m_zoneSize = zoneSize;

	// Create all particles
	switch (c)
	{
	case PT_Snow:
		for (unsigned int i = 0; i < num_particles; i++)
			m_particles.push_back(std::shared_ptr<Particle>(new Snow(zoneSize, smooth)));
		break;
	case PT_Leaf:
		for (unsigned int i = 0; i < num_particles; i++)
			m_particles.push_back(std::shared_ptr<Particle>(new Leaf(zoneSize, smooth)));
        m_mesh.get_graphics()->SetColorAll(Vector3f(0, 0.9f, 0.03f));
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
			m_particles.push_back(std::shared_ptr<Particle>(new Explosion(zoneSize, tex, false, m_power, size)));
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
	m_mesh.tex_clear();

	for (auto& x : m_particles)
		m_mesh.add_tex_offset_static_atlas(x->texture);
}

void ParticleGenerator::LogicUpdate()
{
	int count = 0;
	//std::cout << "X: " << m_particles[0]->position.x << "  Y: " << m_particles[0]->position.y << std::endl;
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
	//int onCam = 0;
	//int notOnCam = 0;
	//int total = m_particles.size();

	// if the number of particles is still the same, update the mmodels instead of re-inserting them
	m_prevModels = m_mesh.get_graphics()->GetMModels().size();
	if (m_prevModels == m_particles.size())
	{
		Transformation t;
		for (int i = 0; i < m_particles.size(); i++)
		{
			auto& x = m_particles[i];
			// If its on camera, update everything like normal
			if (Camera::_currentCam->IsOnCamera(x->position, x->size))
			{
				//onCam++;
				t.SetTranslation(x->position);
				x->SetTrans(t);
				//t.SetScale(x->size * 64.0f);
				m_mesh.get_graphics()->InsertMModels(t, i);
			}
			// if its not on camera, hide the particle and make a cheap update
			// needs to be hidden otherwise the modelMat doesnt change and itll stay frozen on screen until
			//	it comes back into view (logic-wise)
			else
			{
				if (x->matrix.m[2][3] == -100)
					continue;
				x->matrix.m[2][3] = -100;
				m_mesh.get_graphics()->InsertMModels(x->matrix, i);
			}
		}
	}
	// Update all the particle models
	else
	{
		m_mesh.get_graphics()->ClearMModels();

		Transformation t;
		for (auto &x : m_particles)
		{
			t.SetTranslation(x->position);
			x->SetTrans(t);
			t.SetScale(x->size * 64.0f);
			m_mesh.get_graphics()->InsertMModels(t);
		}
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
	Renderer::GetInstance().Add(m_mesh.get_graphics());
}

void ParticleGenerator::Draw()
{
	m_mesh.get_graphics()->Draw();
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
	m_mesh.get_graphics()->SetColorAll(color, alpha);
}
