#include "graphicsComponent.h"

void GraphicsComponent::ReceiveMessage(std::vector<std::string> msg)
{

}

GraphicsComponent::~GraphicsComponent()
{
	UnloadGLResources();
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

	m_originalVertices = std::vector<Vertex>(m_vertices);
	for (auto x : m_originalVertices)
	{
		x.vertex.x *= 64.0f;
		x.vertex.y *= 64.0f;
	}
	for (auto x : m_vertices)
	{
		x.vertex.x *= 64.0f;
		x.vertex.y *= 64.0f;
	}


	Construct();
}

void GraphicsComponent::Construct()
{
	m_outline = false;
	m_direction = dir_Down;
	_instancedDraw = false;
	m_lastMModelSize = 0;

	//When this z is small - transparency fucks up????
	m_pos = Vector3f(0.0f, 0.0f, 0.0f);
	//m_modelMat.SetTranslation(m_pos);

	SetDefaults(m_modelName);

	LoadExternalResources();
	LoadGLResources();
}

GraphicsComponent::GraphicsComponent(std::string modelName, std::string texPath) : m_texture(texPath), m_modelName(modelName)
{
	m_IBO = 0;
	m_VBO = 0;
	m_VAO = 0;
	m_MMBO = 0;
	Construct();
}

GraphicsComponent::GraphicsComponent(std::vector<Vertex>* verts, std::vector<GLuint>* inds, std::string texPath) : m_texture(texPath), m_modelName("NONE")
{
	m_IBO = 0;
	m_VBO = 0;
	m_VAO = 0;
	m_MMBO = 0;

	m_vertices = std::vector<Vertex>(*verts);
	m_indices = std::vector<GLuint>(*inds);

	m_originalVertices = std::vector<Vertex>(m_vertices);

	Construct();
}

void GraphicsComponent::Update()
{
	UpdateTranslation();
	UpdateMModels();
}

void GraphicsComponent::UpdateMModels()
{
	m_mmodels.clear();
	InsertMModels(m_modelMat);
}

void GraphicsComponent::UpdateTranslation()
{
	if (m_modelName == "CENTERED_TILE")
		m_modelMat.SetTranslation(m_pos + Vector3f(0.5f, 0.5f, 0));
	else
		m_modelMat.SetTranslation(m_pos);
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

	// Bind VAO (which binds vbo + IBO)
	glBindVertexArray(m_VAO);

	// Bind texture
	if (withTex)
		ResourceManager::GetInstance().GetTexture(m_texture)->Bind(GL_TEXTURE0);

	// Bind mmbo
	glBindBuffer(GL_ARRAY_BUFFER, m_MMBO);
	if (m_mmodels.size() > m_lastMModelSize)
		glBufferData(GL_ARRAY_BUFFER, sizeof(Mat4f) * m_mmodels.size(), &m_mmodels.at(0), GL_DYNAMIC_DRAW);
	else
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Mat4f) * m_mmodels.size(), &m_mmodels.at(0));

	m_lastMModelSize = sizeof(Mat4f) * m_mmodels.size();

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
	// This doesn't work well if the number of vertices changes - to be kept in mind (glBufferSubData)
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(Vertex), &m_vertices[0]);
}

// Sets the color of all vertices to change the color of the sprite
void GraphicsComponent::SetColorAll(Vector3f color, float alpha)
{
	for (auto& v : m_vertices)
	{
		v.color = color;
		v.alpha = alpha;
	}
	ResetVBO();
}

// Adds 4 model matrices to MModels (does not clear MModels first)
void GraphicsComponent::InsertMModels(Transformation& t)
{
	Vector3f temp = t.GetTranslation();
	temp.x *= OrthoProjInfo::GetRegularInstance().Size;
	temp.y *= OrthoProjInfo::GetRegularInstance().Size;
	t.SetTranslation(temp);
	m_mmodels.insert(m_mmodels.end(), _instancedDraw ? 1 : 4, t.GetWorldTrans());
	//m_mmodels.push_back(Mat4f(t.GetWorldTrans()));
	//m_mmodels.insert(m_mmodels.end(), 1, t.GetWorldTrans());
}


void GraphicsComponent::SetDefaults(std::string name)
{
	m_modelName = name;
	if (name == "NONE" || name == "")
		return;

	m_IBO = 0;
	m_VBO = 0;

	Model::GetInstance().loadModel(name);
	m_modelName = Model::GetInstance().GetName();
	std::vector<float> verts = Model::GetInstance().getVertices();
	std::vector<float> tex = Model::GetInstance().GetTex();

	assert(verts.size() % 3 == 0);//full vertices only plz
								  //assert(tex.size() == (verts.size() % 3) * 2);//2 texcoords per vertex

	int numVertex = (int)verts.size() / 3;

	m_originalVertices = std::vector<Vertex>();

	for (int x = 0; x < numVertex; x++)
	{
		m_originalVertices.push_back(
			Vertex(Vector3f(verts[x * 3], verts[x * 3 + 1], verts[x * 3 + 2]),
				Vector2f(x * 2 < (int)tex.size() ? tex[x * 2] : 0, x * 2 + 1 < (int)tex.size() ? tex[x * 2 + 1] : 0)));
	}

	std::vector<GLuint> indices = Model::GetInstance().getIndices();
	assert(indices.size() % 3 == 0);

	m_indices = std::vector<GLuint>();

	for (auto i : indices)
		m_indices.push_back(i);

	//MathUtils::CalcNormals(m_indices, m_originalVertices);

	m_vertices = std::vector<Vertex>(m_originalVertices);
}


std::string GraphicsComponent::GetName()
{
	return m_modelName;
}

std::vector<Vertex>* GraphicsComponent::GetVertices()
{
	return &m_vertices;
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

void GraphicsComponent::SetDirection(std::shared_ptr<GraphicsComponent> graph)
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


