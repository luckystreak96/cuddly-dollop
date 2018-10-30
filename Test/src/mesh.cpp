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

//void Mesh::init_instanced_tex_drawing(std::vector<Vertex>* verts, std::vector<GLuint>* inds, std::string tex)
//{
//	AddToMesh(*verts, *inds, 3, Vector3f(), tex, 0);
//}

void Mesh::set_placeholder_uv_offset(bool static_atlas)
{
	TextureAtlas* atlas;
	if (static_atlas)
		atlas = &TextureAtlas::m_textureAtlas;
	else
		atlas = &m_texAtlas;
	std::vector<Vector2f> tex = atlas->get_placeholder_uv_offset();

	for (int i = 0; i < tex.size(); i++)
		m_vertexList[i].tex = tex[i];
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
	TextureAtlas* atlas;
	if (index == -1)
		atlas = &TextureAtlas::m_textureAtlas;
	else
		atlas = &m_texAtlas;

	if (tex != "" && !m_textures.count(tex))
		m_textures.emplace(tex, atlas->AddTexture(tex));

	if (tex == "")
			index = 0;

	if (index == -1)
		index = m_textures.at(tex);

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
		atlas->TextureIndexToCoord(index, temp.x, temp.y);

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

void Mesh::add_tex_offset(std::string tex, int index)
{
	m_texCoords.push_back(get_uv_offset_coords(tex, index));
}

void Mesh::change_tex_offset(int pos, std::string tex, int index)
{
	m_texCoords[pos] = get_uv_offset_coords(tex, index);
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

std::vector<Vector2f>* Mesh::get_tex_coords()
{
	return &m_texCoords;
}

Vector2f Mesh::get_uv_offset_coords(std::string tex, int index)
{
	TextureAtlas* atlas;
	if (index == -1)
		atlas = &TextureAtlas::m_textureAtlas;
	else
		atlas = &m_texAtlas;

	//if (tex == "")
	//	return atlas->get_placeholder_uv_offset();

	if (!m_textures.count(tex))
		m_textures.emplace(tex, atlas->AddTexture(tex));

	if (index == -1)
		index = m_textures.at(tex);

	float u = atlas->get_u_offset_coordinate(index);
	float v = atlas->get_v_offset_coordinate(index);

	return Vector2f(u, v);
}
