#include "mesh.h"
#include "define.h"

Mesh::Mesh(unsigned int atlasSize) : m_texAtlas(TextureAtlas(atlasSize)), _instancedDraw(false)
{
	m_indexProgress = 0;
	m_texture = NULL;
	m_progess = 0;
}

Mesh::~Mesh()
{
	if (m_texture)
		delete m_texture;
}

void Mesh::Reset()
{
	m_progess = 0;
	m_vertexList = std::vector<Vertex>();
	m_indices = std::vector<GLuint>();
	m_indexProgress = 0;
	m_textures.clear();
}

void Mesh::AddToMesh(std::vector<Vertex>& verts, const std::vector<GLuint>& inds, int biggestIndex, Vector3f pos, std::string tex, int index)
{
	if (!m_textures.count(tex)) {
		if (index == -1)
			m_textures.emplace(tex, TextureAtlas::m_textureAtlas.AddTexture(tex));
		else
			m_textures.emplace(tex, m_texAtlas.AddTexture(tex));
	}

	std::vector<Vector2f> vecs = {
	Vector2f(0, 0),
	Vector2f(1, 0),
	Vector2f(0, 1),
	Vector2f(1, 1)
	};

	int counter = 0;
	for (auto v : verts)
	{
		Vector2f temp = Vector2f(vecs[counter % 4].x, vecs[counter % 4].y);
		if (index == -1)
			TextureAtlas::m_textureAtlas.TextureIndexToCoord(m_textures.at(tex), temp.x, temp.y);
		else
			m_texAtlas.TextureIndexToCoord(index, temp.x, temp.y);

		v.tex = temp;

		if (!_instancedDraw || m_indexProgress < 4)
			m_vertexList.push_back(v);

		counter++;
	}

	for (auto i : inds)
		if (!_instancedDraw || m_indexProgress < 4)
			m_indices.push_back(i + m_indexProgress);

	//m_models.push_back(t);

	m_indexProgress += biggestIndex + 1;
}

void Mesh::Finalize(std::string name)
{
	//Make sure texture doesnt exist so it doesnt always lag on startup
	struct stat buffer;
	bool textureExists = (stat(name.c_str(), &buffer) == 0);

	if (!textureExists)
		TextureAtlas::m_textureAtlas.Generate(32, false, name);

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
