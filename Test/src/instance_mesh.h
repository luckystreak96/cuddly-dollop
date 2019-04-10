#ifndef INSTANCE_MESH_H__
#define INSTANCE_MESH_H__

#include "mesh.h"

class GraphicsComponent;

// Class that wraps the GraphicsComponent and mesh classes into one
// Takes care of most of the annoying settings and keeps things smooth
// This class takes care of keeping the graphics component up-to-date when it is requested
class instance_mesh
{
public:
	instance_mesh();

	void init_static_atlas(std::shared_ptr<GraphicsComponent> graph, std::string model);
	void init_specific_atlas(std::shared_ptr<GraphicsComponent> graph, std::string model, std::string texture, int atlas_surface_area);

	size_t tex_size();
	void tex_clear();

	void change_tex_offset(int pos, std::string tex, int index = -1);
	void add_tex_offset_static_atlas(std::string tex);
	void add_tex_offset_specific_atlas(std::string tex, int index);

	void generate_colors(ColorGenerator* gen, float alpha);

	std::shared_ptr<GraphicsComponent> get_graphics();
	void set_graphics_position(Vector3f pos);

private:
	std::shared_ptr<GraphicsComponent> m_graphics;
	Mesh m_mesh;

	bool m_is_static_atlas;
	bool m_changed;
};

#endif // !INSTANCE_MESH_H__
