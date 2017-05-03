#include "drawable.h"

Drawable::Drawable(Vector3f pos, std::string modelName, std::string texPath) : m_texture(texPath)
{
	m_pos = pos;
	SetDefaults(modelName);
}

void Drawable::Update()
{
	if (m_modelName == "NONE")
		return;

	m_ground_check = false;//if a collision is about to be detected, its as if it was always on the ground

	Drawable::Move();
	//m_rot.x += 0.002f;
	m_modelMat.SetTranslation(m_pos);
	//m_modelMat.SetRotation(m_rot);
}

bool Drawable::LoadExternalResources()
{
	if (!ResourceManager::GetInstance().LoadTexture(m_texture))
	{
		std::cout << "Texture failed to load!" << std::endl;
		m_external_loaded = false;
		return false;
	}

	m_external_loaded = true;
	return true;
}

bool Drawable::UnloadExternalResources()
{
	m_external_loaded = false;
	std::cout << "UnloadExternalResources (Drawable) isn't implemented!" << std::endl;
	return true;
}


bool Drawable::LoadGLResources()
{
	if (m_modelName == "NONE")
	{
		m_GL_loaded = false;
		return false;
	}

	SetBuffers();
	ResourceManager::GetInstance().LoadGLTexture(m_texture);

	m_GL_loaded = true;
	return true;
}

bool Drawable::UnloadGLResources()
{
	if (m_IBO != 0)
		glDeleteBuffers(1, &m_IBO);
	if (m_VBO != 0)
		glDeleteBuffers(1, &m_VBO);

	m_GL_loaded = false;

	return true;
}

void Drawable::Draw()
{
	if (!m_external_loaded || !m_GL_loaded || !mustDraw || m_modelName == "NONE")
		return;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);//position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(float) * 3));//texcoords
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(float) * 5));//normal
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(float) * 8));//specPow
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(float) * 9));//specIntens
	ResourceManager::GetInstance().GetTexture(m_texture)->Bind(GL_TEXTURE0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	glDrawElements(GL_TRIANGLES, m_indices.size() /** sizeof(GLuint)*/, GL_UNSIGNED_INT, 0);

	if (Globals::DEBUG_DRAW_NORMALS)
	{
		glBegin(GL_LINES);
		glColor3f(1.0, 1.0, 1.0);
		for(Vertex& v : m_vertices)
		{
			glVertex3f(v.vertex.x, v.vertex.y, v.vertex.z);
			glVertex3f(v.vertex.x + v.normal.x, v.vertex.y + v.normal.y, v.vertex.z + v.normal.z);
		}
		glEnd();
	}

	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

void Drawable::SetBuffers()
{
	if (m_IBO == 0)
		glGenBuffers(1, &m_IBO);
	if (m_VBO == 0)
		glGenBuffers(1, &m_VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);//Give it a purpose
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Give it a purpose
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
}

void Drawable::Move()
{
	Vector3f distance = m_velocity * ElapsedTime::GetInstance().GetElapsedTime();
	RelativePosition(distance);
}

void Drawable::SetDefaults(std::string name)
{
	m_modelName = name;
	if (name == "PLAYER")
		int lol = 0;
	if (name == "NONE")
		return;

	m_IBO = 0;
	m_VBO = 0;

	Model::GetInstance().loadModel(name);
	m_modelName = Model::GetInstance().GetName();
	std::vector<float> verts = Model::GetInstance().getVertices();
	std::vector<float> tex = Model::GetInstance().GetTex();

	assert(verts.size() % 3 == 0);//full vertices only plz
	//assert(tex.size() == (verts.size() % 3) * 2);//2 texcoords per vertex

	int numVertex = verts.size() / 3;

	m_originalVertices = std::vector<Vertex>();

	for (int x = 0; x < numVertex; x++)
	{
		m_originalVertices.push_back(
			Vertex(Vector3f(verts[x * 3], verts[x * 3 + 1], verts[x * 3 + 2]), 
				Vector2f(x * 2 < tex.size() ? tex[x * 2] : 0, x * 2 + 1 < tex.size() ? tex[x * 2 + 1] : 0)));//TEXCOORD MOD HERE PLZ
	}

	m_vertices = std::vector<Vertex>(m_originalVertices);

	{
		std::vector<GLuint> indices = Model::GetInstance().getIndices();
		assert(indices.size() % 3 == 0);

		m_indices = std::vector<GLuint>();

		for(auto i : indices)
			m_indices.push_back(i);
	}

	MathUtils::CalcNormals(m_indices, m_vertices);
	SetSpecularStuff();

	SetBoundingBox();
}

void Drawable::SetTranslatedVertices()
{
	if (m_modelName == "PLAYER")
		int lol = 0;
	m_translatedVertices = std::vector<Vertex>(m_vertices);
	for (Vertex& v : m_translatedVertices)
	{
		v.vertex.x += m_pos.x + m_velocity.x * ElapsedTime::GetInstance().GetElapsedTime();
		v.vertex.y += m_pos.y + m_velocity.y * ElapsedTime::GetInstance().GetElapsedTime();
		v.vertex.z += m_pos.z + m_velocity.z * ElapsedTime::GetInstance().GetElapsedTime();
	}
}

void Drawable::CollisionMovement(Vector3f normal, float percent, bool secondCollision, Vector3f traject, int vertIndex, Vector3f poc)
{
	if (!secondCollision)
	{
		const float lower = 0.7f;
		if (normal.x != 0 && abs(traject.x) >= COLLISION_BUFFER_SPACE)
			traject.x *= lower;
		else
			traject.x = 0;

		if (normal.y != 0 && abs(traject.y) >= COLLISION_BUFFER_SPACE)
			traject.y *= lower;
		else
			traject.y = 0;

		if (normal.z != 0 && abs(traject.z) >= COLLISION_BUFFER_SPACE)
			traject.z *= lower;
		else
			traject.z = 0;

		RelativePosition(traject);
	}

	float projectedScalar = m_velocity.VectorProjection(normal);
	Vector3f velChange = normal * -projectedScalar;
	if(!secondCollision)
	{
		m_velocity += velChange;
	}
	else 
	{
		if (velChange.x != 0)
			m_velocity.x = 0;
		if (velChange.y != 0)
			m_velocity.y = 0;
		if (velChange.z != 0)
			m_velocity.z = 0;
	}

	if (normal.z < 0)
		m_was_on_ground = true;
}

void Drawable::SetGrounded(bool was_on_ground)
{
	m_was_on_ground = was_on_ground;
}

std::vector<Vertex> Drawable::GetTranslatedVertices()
{
	return m_translatedVertices;
}

//Find the farthest edges from each other to make the biggest bounding
//box using initial pos and pos + velocity
void Drawable::SetMovementBB()
{
	m_moveBoundingBox.Copy(m_boundingBox);
	m_moveBoundingBox.Add(AABB::Close, 0, m_velocity.z * ElapsedTime::GetInstance().GetElapsedTime());
	m_moveBoundingBox.Add(AABB::Far, 0, m_velocity.z * ElapsedTime::GetInstance().GetElapsedTime());
	m_moveBoundingBox.Add(AABB::Right, 0, m_velocity.x * ElapsedTime::GetInstance().GetElapsedTime());
	m_moveBoundingBox.Add(AABB::Left, 0, m_velocity.x * ElapsedTime::GetInstance().GetElapsedTime());
	m_moveBoundingBox.Add(AABB::Up, 0, m_velocity.y * ElapsedTime::GetInstance().GetElapsedTime());
	m_moveBoundingBox.Add(AABB::Down, 0, m_velocity.y * ElapsedTime::GetInstance().GetElapsedTime());

	//Close Far
	if (abs(m_boundingBox.Get(AABB::Close, 0) - m_moveBoundingBox.Get(AABB::Far, 0)) >
		abs(m_boundingBox.Get(AABB::Far, 0) - m_moveBoundingBox.Get(AABB::Close, 0)))
		m_moveBoundingBox.Set(AABB::Close, 0, m_boundingBox.Get(AABB::Close, 0));
	else
		m_moveBoundingBox.Set(AABB::Far, 0, m_boundingBox.Get(AABB::Far, 0));

	//Left Right
	if (abs(m_boundingBox.Get(AABB::Right, 0) - m_moveBoundingBox.Get(AABB::Left, 0)) >
		abs(m_boundingBox.Get(AABB::Left, 0) - m_moveBoundingBox.Get(AABB::Right, 0)))
		m_moveBoundingBox.Set(AABB::Right, 0, m_boundingBox.Get(AABB::Right, 0));
	else
		m_moveBoundingBox.Set(AABB::Left, 0, m_boundingBox.Get(AABB::Left, 0));

	//Up Down
	if (abs(m_boundingBox.Get(AABB::Up, 0) - m_moveBoundingBox.Get(AABB::Down, 0)) >
		abs(m_boundingBox.Get(AABB::Down, 0) - m_moveBoundingBox.Get(AABB::Up, 0)))
		m_moveBoundingBox.Set(AABB::Up, 0, m_boundingBox.Get(AABB::Up, 0));
	else
		m_moveBoundingBox.Set(AABB::Down, 0, m_boundingBox.Get(AABB::Down, 0));
}


Array2d<float> Drawable::GetMoveBoundingBox()
{
	return m_moveBoundingBox;
}

Vector3f Drawable::BBSize()
{
	float w = m_boundingBox.Get(Right, 0) - m_boundingBox.Get(Left, 0);
	float h = m_boundingBox.Get(Up, 0) - m_boundingBox.Get(Down, 0);
	float d = m_boundingBox.Get(Far, 0) - m_boundingBox.Get(Close, 0);
	return Vector3f(w, h, d);
}

std::string Drawable::GetName()
{
	return m_modelName;
}

void Drawable::SetBoundingBox()
{
	m_boundingBox.Set(AABB::Right, 0, m_vertices[0].vertex.x);
	m_boundingBox.Set(AABB::Left, 0, m_vertices[0].vertex.x);
	m_boundingBox.Set(AABB::Up, 0, m_vertices[0].vertex.y);
	m_boundingBox.Set(AABB::Down, 0, m_vertices[0].vertex.y);
	m_boundingBox.Set(AABB::Close, 0, m_vertices[0].vertex.z);
	m_boundingBox.Set(AABB::Far, 0, m_vertices[0].vertex.z);

	for(auto v : m_vertices)
	{
		//Right
		if (v.vertex.x > m_boundingBox.Get(AABB::Right, 0))
			m_boundingBox.Set(AABB::Right, 0, v.vertex.x);
		//Left
		else if (v.vertex.x < m_boundingBox.Get(AABB::Left, 0))
			m_boundingBox.Set(AABB::Left, 0, v.vertex.x);
		//Up
		if (v.vertex.y > m_boundingBox.Get(AABB::Up, 0))
			m_boundingBox.Set(AABB::Up, 0, v.vertex.y);
		//Down
		else if (v.vertex.y < m_boundingBox.Get(AABB::Down, 0))
			m_boundingBox.Set(AABB::Down, 0, v.vertex.y);
		//Away
		if (v.vertex.z > m_boundingBox.Get(AABB::Far, 0))
			m_boundingBox.Set(AABB::Far, 0, v.vertex.z);
		//Close
		else if (v.vertex.z < m_boundingBox.Get(AABB::Close, 0))
			m_boundingBox.Set(AABB::Close, 0, v.vertex.z);
	}
	MoveBB(m_pos);
}

void Drawable::MoveBB(Vector3f distance)
{
	m_boundingBox.Set(AABB::Right, 0, m_boundingBox.Get(AABB::Right, 0) + distance.x);
	m_boundingBox.Set(AABB::Left, 0, m_boundingBox.Get(AABB::Left, 0) + distance.x);
	m_boundingBox.Set(AABB::Up, 0, m_boundingBox.Get(AABB::Up, 0) + distance.y);
	m_boundingBox.Set(AABB::Down, 0, m_boundingBox.Get(AABB::Down, 0) + distance.y);
	m_boundingBox.Set(AABB::Close, 0, m_boundingBox.Get(AABB::Close, 0) + distance.z);
	m_boundingBox.Set(AABB::Far, 0, m_boundingBox.Get(AABB::Far, 0) + distance.z);
}

Array2d<float> Drawable::GetBoundingBox()
{
	return m_boundingBox;
}

void Drawable::RelativePosition(Vector3f movementPos)
{
	m_pos += movementPos;
	MoveBB(movementPos);
}

void Drawable::AbsolutePosition(Vector3f absolutePos)
{
	absolutePos.x = absolutePos.x == -1 ? m_pos.x : absolutePos.x;
	absolutePos.y = absolutePos.y == -1 ? m_pos.y : absolutePos.y;
	absolutePos.z = absolutePos.z == -1 ? m_pos.z : absolutePos.z;
	m_pos = absolutePos;
	SetBoundingBox();
}

void Drawable::SetSpecularStuff()
{
	for(Vertex& v : m_vertices)
	{
		v.SpecularPower = m_specularPower;
		v.SpecularIntensity = m_specularIntensity;
	}
}

std::vector<Vertex> Drawable::GetVertices()
{
	return m_vertices;
}

std::vector<GLuint> Drawable::GetIndices()
{
	return m_indices;
}

int Drawable::GetHighestIndex()
{
	GLuint highest = 0;
	for (auto i : m_indices)
		highest = i > highest ? i : highest;

	return highest;
}

void Drawable::DesiredMove()
{
	SetMovementBB();
}

std::string Drawable::GetTexture()
{
	return m_texture;
}