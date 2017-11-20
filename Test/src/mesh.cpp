#include "mesh.h"

Mesh::Mesh(unsigned int atlasSize) : m_texAtlas(TextureAtlas(atlasSize))
{
	m_IBO = 0;
	m_VBO = 0;
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

void Mesh::AddToMesh(std::vector<Vertex>& verts, std::vector<GLuint>& inds, int biggestIndex, Vector3f pos, std::string tex, int index)
{
	if (!m_textures.count(tex)) {
		//m_textures.erase(tex);
		if (index == -1)
			m_textures.emplace(tex, TextureAtlas::m_textureAtlas.AddTexture(tex));
		else
			m_textures.emplace(tex, m_texAtlas.AddTexture(tex));
	}
	//if (m_textures.emplace(tex, 0).second) {
	//	m_textures.erase(tex);
	//	m_textures.emplace(tex, TextureAtlas::m_textureAtlas.AddTexture(tex));
	//}

	for (auto v : verts)
	{
		Vertex temp = Vertex(v);
		//temp.vertex += pos;
		//if (t != Transformation())
		//{
		//	t.SetTranslation(temp.vertex.x, temp.vertex.y, temp.vertex.z);
		//	Mat4f mat = t.GetWorldTrans();
		//	temp.vertex.x = mat.m[0][3];
		//	temp.vertex.y = mat.m[1][3];
		//	temp.vertex.z = mat.m[2][3];
		//}
		if (index == -1)
			TextureAtlas::m_textureAtlas.TextureIndexToCoord(m_textures.at(tex), temp.tex.x, temp.tex.y);
		else
			m_texAtlas.TextureIndexToCoord(index, temp.tex.x, temp.tex.y);

		m_vertexList.push_back(temp);
	}

	for (auto i : inds)
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