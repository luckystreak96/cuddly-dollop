#include "graphicsComponent.h"

void GraphicsComponent::ReceiveMessage(std::vector<std::string> msg)
{

}

GraphicsComponent::~GraphicsComponent()
{
	delete m_models;
}

void GraphicsComponent::Construct()
{
	m_pos = Vector3f(2.0f, 2.0f, 5.0f);
	m_modelMat.SetTranslation(m_pos);

	SetDefaults(m_modelName);

	LoadExternalResources();
	LoadGLResources();
}

GraphicsComponent::GraphicsComponent(std::string modelName, std::string texPath) : m_texture(texPath), m_modelName(modelName)
{
	Construct();
}

GraphicsComponent::GraphicsComponent(std::vector<Vertex> verts, std::vector<GLuint> inds, std::string texPath) : m_texture(texPath), m_modelName("NONE")
{
	m_IBO = 0;
	m_VBO = 0;

	m_vertices = std::vector<Vertex>(verts);
	m_indices = std::vector<GLuint>(inds);

	MathUtils::CalcNormals(m_indices, m_vertices);

	m_originalVertices = std::vector<Vertex>(m_vertices);

	Construct();
}

void GraphicsComponent::Update()
{
	m_modelMat.SetTranslation(m_pos);

	if (m_models != NULL)
		delete m_models;

	m_models = new std::vector<Vector3f>();
	m_models->insert(m_models->end(), 4, m_pos);

	//Might not be ideal to have this here
	BasicEffect::GetInstance().Enable();
	BasicEffect::GetInstance().SetModelPosition(&m_modelMat.GetWorldTrans().m[0][0]);
}

bool GraphicsComponent::LoadExternalResources()
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
	ResourceManager::GetInstance().LoadGLTexture(m_texture);

	m_GL_loaded = true;
	return true;
}

bool GraphicsComponent::UnloadGLResources()
{
	if (m_IBO != 0)
		glDeleteBuffers(1, &m_IBO);
	if (m_VBO != 0)
		glDeleteBuffers(1, &m_VBO);

	m_GL_loaded = false;

	return true;
}

void GraphicsComponent::Draw(bool withTex)
{
	if (!m_external_loaded || !m_GL_loaded || !mustDraw || (m_modelName == "NONE" && m_vertices.size() <= 0))
		return;

	for (int i = 0; i < 6; i++)//0 to 5
		glEnableVertexAttribArray(i);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);//vertex position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(float) * 3));//texcoords
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(float) * 5));//normal
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(float) * 8));//specPow
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(float) * 9));//specIntens

	if (withTex)
		ResourceManager::GetInstance().GetTexture(m_texture)->Bind(GL_TEXTURE0);

	glBindBuffer(GL_ARRAY_BUFFER, m_MBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * m_models->size(), &m_models->at(0), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size() /** sizeof(GLuint)*/, GL_UNSIGNED_INT, 0);

	if (Globals::DEBUG_DRAW_NORMALS)
	{
		glBegin(GL_LINES);
		glColor3f(1.0, 1.0, 1.0);
		for (Vertex& v : m_vertices)
		{
			glVertex3f(v.vertex.x, v.vertex.y, v.vertex.z);
			glVertex3f(v.vertex.x + v.normal.x, v.vertex.y + v.normal.y, v.vertex.z + v.normal.z);
		}
		glEnd();
	}

	for (int i = 5; i >= 0; i--)//5 to 0
		glEnableVertexAttribArray(i);
}

void GraphicsComponent::SetBuffers()
{
	if (m_IBO == 0)
		glGenBuffers(1, &m_IBO);
	if (m_VBO == 0)
		glGenBuffers(1, &m_VBO);
	if (m_MBO == 0)
		glGenBuffers(1, &m_MBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);//Give it a purpose
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Give it a purpose
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
}

void GraphicsComponent::ResetVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Give it a purpose
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
}

void GraphicsComponent::SetDefaults(std::string name)
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

	int numVertex = (int)verts.size() / 3;

	m_originalVertices = std::vector<Vertex>();

	for (int x = 0; x < numVertex; x++)
	{
		m_originalVertices.push_back(
			Vertex(Vector3f(verts[x * 3], verts[x * 3 + 1], verts[x * 3 + 2]),
				Vector2f(x * 2 < tex.size() ? tex[x * 2] : 0, x * 2 + 1 < tex.size() ? tex[x * 2 + 1] : 0)));
	}


	{
		std::vector<GLuint> indices = Model::GetInstance().getIndices();
		assert(indices.size() % 3 == 0);

		m_indices = std::vector<GLuint>();

		for (auto i : indices)
			m_indices.push_back(i);
	}

	MathUtils::CalcNormals(m_indices, m_originalVertices);
	m_vertices = std::vector<Vertex>(m_originalVertices);

	//SetBoundingBox();
}


std::string GraphicsComponent::GetName()
{
	return m_modelName;
}

std::vector<Vertex> GraphicsComponent::GetVertices()
{
	return m_vertices;
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