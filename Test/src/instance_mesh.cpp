#include "instance_mesh.h"

#include "graphics_component.h"

instance_mesh::instance_mesh() : m_changed(false)
{
}

void instance_mesh::init_static_atlas(std::shared_ptr<GraphicsComponent> graph, std::string model)
{
	m_graphics = graph;

	m_mesh.add_instanced_base_to_mesh_static_atlas(model);
//	m_graphics->SetNewBuffers(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices());
	m_graphics->get_buffers()->update_vertex_buffer()->assign(m_mesh.GetMeshVertices()->begin(), m_mesh.GetMeshVertices()->end());
	m_graphics->get_buffers()->update_index_buffer()->assign(m_mesh.GetMeshIndices()->begin(), m_mesh.GetMeshIndices()->end());

//	m_graphics->_instancedDraw = true;
//	m_graphics->_instanced_tex_coord_draw = true;

	m_is_static_atlas = true;
}

void instance_mesh::init_specific_atlas(std::shared_ptr<GraphicsComponent> graph, std::string model, std::string texture, int atlas_surface_area)
{
	m_graphics = graph;

	m_mesh = Mesh(atlas_surface_area);
	m_mesh.add_instanced_base_to_mesh_specific_atlas(model, texture);
//	m_graphics->SetNewBuffers(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices());
	m_graphics->get_buffers()->update_vertex_buffer()->assign(m_mesh.GetMeshVertices()->begin(), m_mesh.GetMeshVertices()->end());
	m_graphics->get_buffers()->update_index_buffer()->assign(m_mesh.GetMeshIndices()->begin(), m_mesh.GetMeshIndices()->end());

//	m_graphics->_instancedDraw = true;
//	m_graphics->_instanced_tex_coord_draw = true;

	m_is_static_atlas = false;
}

size_t instance_mesh::tex_size()
{
	return m_mesh.get_tex_coords()->size();
}

void instance_mesh::tex_clear()
{
	m_mesh.get_tex_coords()->clear();
	m_changed = true;
}

void instance_mesh::change_tex_offset(int pos, std::string tex, int index)
{
	m_mesh.change_tex_offset(pos, tex, index);
	m_changed = true;
}

void instance_mesh::add_tex_offset_static_atlas(std::string tex)
{
	m_mesh.add_tex_offset_static_atlas(tex);
	m_changed = true;
}

void instance_mesh::add_tex_offset_specific_atlas(std::string tex, int index)
{
	m_mesh.add_tex_offset_specific_atlas(tex, index);
	m_changed = true;
}

std::shared_ptr<GraphicsComponent> instance_mesh::get_graphics()
{
	if (m_changed)
	{
		m_changed = false;
		m_graphics->get_buffers()->update_tex_coord_buffer()->assign(m_mesh.get_tex_coords()->begin(), m_mesh.get_tex_coords()->end());
	}

	return m_graphics;
}

void instance_mesh::set_graphics_position(Vector3f pos)
{
	m_graphics->set_position(pos);
}

void instance_mesh::generate_colors(ColorGenerator *gen, float alpha) {
	m_mesh.generate_colors(gen, alpha);
	m_graphics->SetColorAll(Vector3f(0), 1.0f);
	m_graphics->get_buffers()->update_vector3f_buffer(BT_Color)->assign(m_mesh.get_colors()->begin(), m_mesh.get_colors()->end());

}

