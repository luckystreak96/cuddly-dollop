#include "mesh.h"

Mesh::Mesh() : m_texAtlas(TextureAtlas(8))
{
	m_IBO = 0;
	m_VBO = 0;
	m_indices = std::vector<GLuint>();
	m_vertexList = std::vector<Vertex>();
	m_models = std::vector<Transformation*>();
	m_indexProgress = 0;
	m_texture = NULL;
	m_progess = 0;
}

Mesh::~Mesh()
{
}

void Mesh::Reset()
{
	m_progess = 0;
	m_vertexList = std::vector<Vertex>();
	m_indices = std::vector<GLuint>();
	m_indexProgress = 0;
}

void Mesh::AddToMesh(std::vector<Vertex>& verts, std::vector<GLuint>& inds, int biggestIndex, Vector3f pos, std::string tex, Transformation* t)
{
	if (m_textures.emplace(tex, 0).second) {
		m_textures.erase(tex);
		m_textures.emplace(tex, m_texAtlas.AddTexture(tex));
	}

	for (auto v : verts)
	{
		Vertex temp = Vertex(v);
		temp.vertex += pos;
		m_texAtlas.TextureIndexToCoord(m_textures.at(tex), temp.tex.x, temp.tex.y);
		m_vertexList.push_back(temp);
	}

	for (auto i : inds)
		m_indices.push_back(i + m_indexProgress);

	m_models.push_back(t);

	m_indexProgress += biggestIndex + 1;
}

void Mesh::Finalize(std::string name)
{
	//Make sure texture doesnt exist so it doesnt always lag on startup
	struct stat buffer;
	bool textureExists = (stat(name.c_str(), &buffer) == 0);

	if (!textureExists)
		m_texAtlas.Generate(1024, false, name);

	ResourceManager::GetInstance().LoadTexture(name);
}

std::vector<Vertex>* Mesh::GetMeshVertices()
{
	return &m_vertexList;
}

std::vector<GLuint>* Mesh::GetMeshIndices()
{
	return &m_indices;
}