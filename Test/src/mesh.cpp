#include "mesh.h"

#include <sys/stat.h>
#include <iostream>
#include "resource_manager.h"
#include "model.h"

Mesh::Mesh() : m_texAtlas(TextureAtlas(8)), m_indexProgress(0)
{
}

void Mesh::init_static_atlas(GraphicsComponent* graph, std::string model) {
	m_graphics = graph;
	add_instanced_base_to_mesh_static_atlas(model);
}

void Mesh::init_specific_atlas(GraphicsComponent* graph, std::string model, std::string texture,
							   int atlas_surface_area) {
	m_graphics = graph;
	m_texAtlas = TextureAtlas(atlas_surface_area);
	add_instanced_base_to_mesh_specific_atlas(model, texture);
}

void Mesh::set_placeholder_uv_offset(bool static_atlas)
{
	TextureAtlas* atlas;
	if (static_atlas)
		atlas = &TextureAtlas::m_textureAtlas;
	else
		atlas = &m_texAtlas;
	std::vector<Vector2f> tex = atlas->get_placeholder_uv_offset();

	std::vector<Vertex>* verts = m_graphics->get_buffers()->update_vertex_buffer();
	for (int i = 0; i < tex.size(); i++)
		verts->at(i).tex = tex[i];
}

void Mesh::add_instanced_base_to_mesh_specific_atlas(std::string model, std::string tex)
{
	add_instanced_base_to_mesh(model, tex, false);
}

void Mesh::add_instanced_base_to_mesh_static_atlas(std::string model)
{
	add_instanced_base_to_mesh(model, "", true);
}

void Mesh::add_instanced_base_to_mesh(std::string model, std::string tex, bool use_global_tileset)
{
	Model::GetInstance().loadModel(model);
	std::vector<Vertex> verts = Model::GetInstance().getVertexVertices();
	std::vector<GLuint> inds = Model::GetInstance().getIndices();

	// Clear the buffers in case the default values are incorrect
	m_graphics->get_buffers()->update_vertex_buffer()->clear();
	m_graphics->get_buffers()->update_index_buffer()->clear();

	for (auto& x : verts)
	{
		x.vertex.x *= 64.0f;
		x.vertex.y *= 64.0f;
	}

	if (use_global_tileset)
		tex = "";
	else
		ResourceManager::GetInstance().LoadTexture(tex);

	AddToMesh(verts, inds, 3, Vector3f(), tex, use_global_tileset ? -1 : 0);
	set_placeholder_uv_offset(use_global_tileset);
}

void Mesh::add_tex_offset_static_atlas(std::string tex)
{
	add_tex_offset(tex);
}

void Mesh::add_tex_offset_specific_atlas(std::string tex, int index)
{
	add_tex_offset(tex, index);
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
	std::vector<Vertex>* vert_buf = m_graphics->get_buffers()->update_vertex_buffer();
	for (auto v : verts)
	{
		Vector2f temp = Vector2f(vecs[counter % 4].x, vecs[counter % 4].y);
		atlas->TextureIndexToCoord(index, temp.x, temp.y);

		v.tex = temp;

		if (m_indexProgress < 4)
			vert_buf->push_back(v);

		counter++;
	}

	std::vector<GLuint>* ind_buf = m_graphics->get_buffers()->update_index_buffer();
	for (auto i : inds)
		if (m_indexProgress < 4)
			ind_buf->push_back(i + m_indexProgress);

	//m_models.push_back(t);

	m_indexProgress += biggestIndex + 1;
}

void Mesh::add_tex_offset(std::string tex, int index)
{
	std::vector<Vector2f>* tex_buf = m_graphics->get_buffers()->update_tex_coord_buffer();
	tex_buf->push_back(get_uv_offset_coords(tex, index));
}

void Mesh::change_tex_offset(int pos, std::string tex, int index)
{
	std::vector<Vector2f>* tex_buf = m_graphics->get_buffers()->update_tex_coord_buffer();
	tex_buf->at(pos) = get_uv_offset_coords(tex, index);
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

void Mesh::generate_colors(ColorGenerator *gen, float alpha) {
	std::vector<Vector3f>* col_buf = m_graphics->get_buffers()->update_vector3f_buffer(BT_Color);
	std::vector<Vector2f>* tex_buf = m_graphics->get_buffers()->update_tex_coord_buffer();

	// sets the vertex colors to black (the shader does a max on the vertex color value and the color_buffer value
	m_graphics->SetColorAll(Vector3f(0), 1.0f);

	// Clear colors
	col_buf->clear();

	// Fill buffer with new colors
    for(int i = 0; i < tex_buf->size(); i++)
    	col_buf->push_back(gen->next());
}

size_t Mesh::tex_size() {
    return m_graphics->get_buffers()->update_tex_coord_buffer()->size();
}

void Mesh::tex_clear() {
    m_graphics->get_buffers()->update_tex_coord_buffer()->clear();
}

GraphicsComponent* Mesh::get_graphics() {
    return m_graphics;
}

