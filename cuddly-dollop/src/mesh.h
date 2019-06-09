#ifndef MESH_H__
#define MESH_H__

#include <string>
#include <map>
#include "textureatlas.h"
#include "color_generator.h"
#include "graphics_component.h"

// Layer of a GraphicsComponent that encapsulates texture atlas
class Mesh
{
public:
	Mesh();

	// Initialization
	void init_static_atlas(GraphicsComponent* graph, std::string model);
	void init_specific_atlas(GraphicsComponent* graph, std::string model, std::string texture, int atlas_surface_area);

    void add_instanced_base_to_mesh_specific_atlas(std::string model, std::string tex);
    void add_instanced_base_to_mesh_static_atlas(std::string model);

	void AddToMesh(std::vector<Vertex>& verts, const std::vector<GLuint>& inds, int biggestIndex, Vector3f pos, std::string tex, int index = -1);
	void add_tex_offset_static_atlas(std::string tex);
	void add_tex_offset_specific_atlas(std::string tex, int index);
	void change_tex_offset(int pos, std::string tex, int index = -1);

	// Sets basic uv tex_coords (bottom right corner) to the 4 first vertices in m_vertexlist
	void set_placeholder_uv_offset(bool static_atlas);
	void Finalize(std::string name);
	Vector2f get_uv_offset_coords(std::string tex, int index = -1);
	void generate_colors(ColorGenerator* gen, float alpha);

	// Diverse
	size_t tex_size();
	void tex_clear();
    GraphicsComponent* get_graphics();

private:
	void add_instanced_base_to_mesh(std::string model, std::string tex, bool use_global_tileset);
	void add_tex_offset(std::string tex, int index = -1);

private:
	GraphicsComponent* m_graphics;
	int m_indexProgress;
	TextureAtlas m_texAtlas;
	std::map<std::string, unsigned int> m_textures;
};

#endif
