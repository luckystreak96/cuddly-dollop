#include "graphicsComponent.h"
#include "model.h"
#include <algorithm>

float GraphicsComponent::_persRotation = -0.4f;
float GraphicsComponent::_orthoRotation = -0.0225f;
int GraphicsComponent::counter = 0;

void GraphicsComponent::ReceiveMessage(std::vector<std::string> msg)
{

}

float GraphicsComponent::GetProjectionRotation()
{
	return Transformation::perspectiveOrtho ? _orthoRotation : _persRotation;
}

GraphicsComponent::~GraphicsComponent()
{
	UnloadGLResources();
	//std::cout << --counter << " GraphicsComponents exist." << std::endl;
}

void GraphicsComponent::SetNewBuffers(std::vector<Vertex>* verts, std::vector<GLuint>* inds)
{
	_must_update_vbo_ibo = true;

	m_vertices = std::vector<Vertex>(*verts);
	m_indices = std::vector<GLuint>(*inds);
	m_lastMModelSize = 0;
	return;
	SetBuffers();
}

void GraphicsComponent::FullReset(std::vector<Vertex>* verts, std::vector<GLuint>* inds)
{
	UnloadGLResources();

	m_IBO = 0;
	m_VBO = 0;
	m_VAO = 0;
	m_MMBO = 0;

	m_vertices = std::vector<Vertex>(*verts);
	m_indices = std::vector<GLuint>(*inds);

	//MathUtils::CalcNormals(m_indices, m_vertices);

	//m_originalVertices = std::vector<Vertex>(m_vertices);
	//for (auto x : m_originalVertices)
	//{
	//	x.vertex.x *= 64.0f;
	//	x.vertex.y *= 64.0f;
	//}
	for (auto x : m_vertices)
	{
		x.vertex.x *= 64.0f;
		x.vertex.y *= 64.0f;
	}


	Construct();
}

void GraphicsComponent::Construct()
{
	m_IBO = 0;
	m_VBO = 0;
	m_VAO = 0;
	m_MMBO = 0;

	_must_update_vbo = false;
	_outline = false;
	m_direction = dir_Down;
	m_lastInteraction = m_direction;
	_instancedDraw = false;
	m_lastMModelSize = 0;
	m_mlMatLoc = 0;

	//When this z is small - transparency fucks up????
	m_pos = Vector3f(0.0f, 0.0f, 0.0f);
	//m_modelMat.SetTranslation(m_pos);

	SetDefaults(m_modelName);

	LoadExternalResources();
	LoadGLResources();
}

GraphicsComponent::GraphicsComponent(std::string modelName, std::string texPath) : m_texture(texPath), m_modelName(modelName)
, _mModelsNoReplace(false)
{
	//std::cout << ++counter << " GraphicsComponents exist." << std::endl;
	Construct();
}

GraphicsComponent::GraphicsComponent(std::vector<Vertex>* verts, std::vector<GLuint>* inds, std::string texPath) : m_texture(texPath), m_modelName("NONE"),
_mModelsNoReplace(false)
{
	//std::cout << ++counter << " GraphicsComponents exist." << std::endl;
	m_vertices = std::vector<Vertex>(*verts);
	m_indices = std::vector<GLuint>(*inds);

	//m_originalVertices = std::vector<Vertex>(m_vertices);

	Construct();
}

void GraphicsComponent::Update()
{
	if (UpdateTranslation() || m_updateMModels || m_mmodels.size() == 0 || OrthoProjInfo::GetRegularInstance().changed)
	{
		UpdateMModels();
		m_updateMModels = false;
	}
}

void GraphicsComponent::SetScale(float x, float y, float z)
{
	m_modelMat.SetScale(x, y, z);
	m_updateMModels = true;
}

void GraphicsComponent::SetScale(Vector3f s)
{
	m_modelMat.SetScale(s);
	m_updateMModels = true;
}


void GraphicsComponent::UpdateMModels()
{
	if (_mModelsNoReplace && m_mmodels.size() > 0)
		InsertMModels(m_modelMat, 0);
	else
	{
		m_mmodels.clear();
		InsertMModels(m_modelMat);
	}
}

bool GraphicsComponent::UpdateTranslation()
{
	Vector3f translation;
	Vector3f offset;

	if (m_modelName == "CENTERED_TILE")
		offset = Vector3f(0.5f, 0.5f, 0.0f);
	//else if (m_modelName == "SCREEN")
	//	translation = m_pos/* + Vector3f(0.5f, 0.5f, 0)*/;
	//else
	//	translation = m_pos;

	translation = m_pos + offset;

	if (translation == m_modelMat.GetTranslation())
		return false;

	m_modelMat.SetTranslation(translation);

	return true;
}

bool GraphicsComponent::LoadExternalResources()
{
	if (!ResourceManager::GetInstance().LoadTexture(m_texture))
	{
		// If its not something stupid, send error message
		if (!(m_texture == "" || m_texture[m_texture.size() - 1] == '\\' || m_texture[m_texture.size() - 1] == '/'))
			std::cout << "Texture '" << m_texture << "' could not load" << std::endl;
		m_external_loaded = false;
		return false;
	}

	m_external_loaded = true;
	return true;
}

bool GraphicsComponent::UnloadExternalResources()
{
	m_external_loaded = false;
	std::cout << "UnloadExternalResources (GraphicsComponent) isn't implemented!" << std::endl;
	return true;
}


bool GraphicsComponent::LoadGLResources()
{
	if (m_modelName == "NONE" && m_vertices.size() <= 0)
	{
		m_GL_loaded = false;
		return false;
	}

	SetBuffers();
	//ResourceManager::GetInstance().LoadGLTexture(m_texture);

	m_GL_loaded = true;
	return true;
}

void GraphicsComponent::ApplyModelToVertices()
{
	for (auto& x : m_vertices)
	{
		x.vertex *= m_modelMat.GetScale();
		x.vertex += m_pos;
	}
}

bool GraphicsComponent::UnloadGLResources()
{
	if (m_IBO != 0)
		glDeleteBuffers(1, &m_IBO);
	if (m_VBO != 0)
		glDeleteBuffers(1, &m_VBO);
	if (m_VAO != 0)
		glDeleteVertexArrays(1, &m_VAO);
	if (m_MMBO != 0)
		glDeleteBuffers(1, &m_MMBO);

	m_GL_loaded = false;

	return true;
}

void GLErrorCheck()
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		std::cout << glewGetErrorString(err) << std::endl;
}

void GraphicsComponent::Draw(bool withTex)
{
	// Must be drawn?
	if (!m_external_loaded || !m_GL_loaded || !mustDraw || (m_modelName == "NONE" && m_vertices.size() <= 0))
		return;

	if (_must_update_vbo_ibo)
	{
		SetBuffers();
		_must_update_vbo_ibo = false;
	}
	else if (_must_update_vbo)
	{
		// This doesn't work well if the number of vertices changes - to be kept in mind (glBufferSubData)
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(Vertex), &m_vertices[0]);
		_must_update_vbo = false;
	}


	// Bind VAO (which binds vbo + IBO)
	glBindVertexArray(m_VAO);

	// Bind texture
	if (withTex)
	{
		Texture* tex = ResourceManager::GetInstance().GetTexture(m_texture);
		if (tex)
			tex->Bind(GL_TEXTURE0);
	}

	// Bind mmbo
	glBindBuffer(GL_ARRAY_BUFFER, m_MMBO);
	if (m_mmodelsUpdated)
	{
		//static int modelUpdate = 0;
		//modelUpdate++;

		//std::cout << modelUpdate << std::endl;

		if (m_mmodels.size() * sizeof(Mat4f) > m_lastMModelSize)
			glBufferData(GL_ARRAY_BUFFER, sizeof(Mat4f) * m_mmodels.size(), &m_mmodels.at(0), GL_DYNAMIC_DRAW);
		else
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Mat4f) * m_mmodels.size(), &m_mmodels.at(0));

		m_lastMModelSize = sizeof(Mat4f) * m_mmodels.size();
		m_mmodelsUpdated = false;
	}

	for (int i = 4; i < 8; i++)
	{
		glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (const GLvoid*)(sizeof(float) * (i - 4) * 4));
		glVertexAttribDivisor(i, _instancedDraw ? 1 : 0);
	}

	// Draw
	if (_instancedDraw)
	{
		glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0, m_mmodels.size());
	}
	else
	{
		glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
	}

	//Unbind VAO
	glBindVertexArray(0);
}

void GraphicsComponent::SetupVAO()
{
	// Bind VAO
	glBindVertexArray(m_VAO);

	// Setup binds
	for (int i = 0; i < 8; i++)
		glEnableVertexAttribArray(i);

	// Bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);//vertex position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(float) * 3));//texcoords
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(float) * 5));//color
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(float) * 8));//alpha

	// Unbind VAO
	glBindVertexArray(0);
}

void GraphicsComponent::SetBuffers()
{
	if (m_IBO == 0)
		glGenBuffers(1, &m_IBO);
	if (m_VBO == 0)
		glGenBuffers(1, &m_VBO);
	if (m_MMBO == 0)
		glGenBuffers(1, &m_MMBO);
	if (m_VAO == 0)
		glGenVertexArrays(1, &m_VAO);

	SetupVAO();
}

void GraphicsComponent::ResetVBO()
{
	_must_update_vbo = true;
	return;

	// Crashes if size is 0 lol
	if (m_vertices.size() <= 0)
		return;

	// This doesn't work well if the number of vertices changes - to be kept in mind (glBufferSubData)
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(Vertex), &m_vertices[0]);
}

// Sets the color of all vertices to change the color of the sprite
void GraphicsComponent::SetColorAll(Vector3f color, float alpha)
{
	if (m_vertices.size() && m_vertices[0].color == color && m_vertices[0].alpha == alpha)
		return;

	Vertex::SetColorAll(m_vertices.begin(), m_vertices.end(), color, alpha);
	//for (auto& v : m_vertices)
	//{
	//	v.color = color;
	//	v.alpha = alpha;
	//}
	ResetVBO();
}

// Adds 4 model matrices to MModels (does not clear MModels first)
void GraphicsComponent::InsertMModels(Transformation& t)
{
	Vector3f reminder = t.GetTranslation();
	Vector3f temp = t.GetTranslation();
	temp.x *= OrthoProjInfo::GetRegularInstance().Size;
	temp.y *= OrthoProjInfo::GetRegularInstance().Size;
	if (!Transformation::perspectiveOrtho)
		temp.z *= OrthoProjInfo::GetRegularInstance().Size;
	else
		temp.z *= 1.3f;
	t.SetTranslation(temp);
	int num = _instancedDraw ? 1 : 4;
	m_mmodels.insert(m_mmodels.end(), num, t.GetWorldTrans());

	t.SetTranslation(reminder);
	m_mmodelsUpdated = true;
	//m_mmodels.push_back(Mat4f(t.GetWorldTrans()));
	//m_mmodels.insert(m_mmodels.end(), 1, t.GetWorldTrans());
}

// Updates 4 model matrices to MModels instead of appending
void GraphicsComponent::InsertMModels(Transformation& t, int position)
{
	Vector3f reminder = t.GetTranslation();
	Vector3f temp = t.GetTranslation();
	temp.x *= OrthoProjInfo::GetRegularInstance().Size;
	temp.y *= OrthoProjInfo::GetRegularInstance().Size;
	if (!Transformation::perspectiveOrtho)
		temp.z *= OrthoProjInfo::GetRegularInstance().Size;
	else
		temp.z *= 1.3f;
	t.SetTranslation(temp);
	t.SetRotation(t.GetRotation());
	int num = _instancedDraw ? 1 : 4;
	Mat4f& trans = t.GetWorldTrans();
	for (int i = 0; i < num; i++)
		m_mmodels[position * 4 + i] = trans;
	t.SetTranslation(reminder);
	m_mmodelsUpdated = true;
	//m_mmodels.push_back(Mat4f(t.GetWorldTrans()));
	//m_mmodels.insert(m_mmodels.end(), 1, t.GetWorldTrans());
}

// Updates 4 model matrices to MModels instead of appending + uses a pre-computed matrix
void GraphicsComponent::InsertMModels(Mat4f& mat, int position)
{
	float size = OrthoProjInfo::GetRegularInstance().Size;
	mat.m[0][3] *= size;
	mat.m[1][3] *= size;
	mat.m[2][3] *= size;
	int num = _instancedDraw ? 1 : 4;
	for (int i = 0; i < num; i++)
		m_mmodels[position * 4 + i] = mat;
	m_mmodelsUpdated = true;
	//m_mmodels.push_back(Mat4f(t.GetWorldTrans()));
	//m_mmodels.insert(m_mmodels.end(), 1, t.GetWorldTrans());
}


void GraphicsComponent::SetDefaults(std::string name)
{
	m_modelName = name;
	if (name == "NONE" || name == "")
		return;

	//m_IBO = 0;
	//m_VBO = 0;

	Model::GetInstance().loadModel(name);
	m_modelName = Model::GetInstance().GetName();

	m_vertices = std::vector<Vertex>(Model::GetInstance().getVertexVertices());
	//m_vertices = std::vector<Vertex>(m_originalVertices);
	//assert(m_vertices.size() % 3 == 0);//full vertices only plz

	m_indices = std::vector<GLuint>(Model::GetInstance().getIndices());
	assert(m_indices.size() % 3 == 0);

	//MathUtils::CalcNormals(m_indices, m_originalVertices);
}


std::string GraphicsComponent::GetName()
{
	return m_modelName;
}

std::vector<Vertex>* GraphicsComponent::GetVertices()
{
	return &m_vertices;
}

void GraphicsComponent::ClearMModels()
{
	m_mmodels.clear();
	m_test.clear();
}

std::vector<GLuint> GraphicsComponent::GetIndices()
{
	return m_indices;
}

int GraphicsComponent::GetHighestIndex()
{
	GLuint highest = 0;
	for (auto i : m_indices)
		highest = i > highest ? i : highest;

	return highest;
}

std::string GraphicsComponent::GetTexture()
{
	return m_texture;
}

void GraphicsComponent::SetTexture(std::string newTex)
{
	m_texture = newTex;
	LoadExternalResources();
}

Vector3f GraphicsComponent::GetPos() { return m_pos; }

Direction GraphicsComponent::GetDirection() { return m_direction; }
void GraphicsComponent::SetDirection(Direction dir) { m_direction = dir; }

void GraphicsComponent::SetDirection(GraphComp_ptr graph)
{
	float x = m_pos.x;
	float y = m_pos.y;
	float ox = graph->GetPos().x;
	float oy = graph->GetPos().y;

	if (abs(x - ox) > abs(y - oy))
	{
		//x is closer

		if (x - ox < 0)
			m_direction = dir_Right;
		else
			m_direction = dir_Left;

	}
	else
	{
		//y is closer

		//your under the other guy, turn up
		if (y - oy < 0)
			m_direction = dir_Up;
		else
			m_direction = dir_Down;
	}

	m_lastInteraction = m_direction;
}

std::vector<Mat4f>& GraphicsComponent::GetMModels()
{
	return m_mmodels;
}


