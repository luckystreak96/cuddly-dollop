#include "instance_mesh.h"

#include "graphicsComponent.h"

instance_mesh::instance_mesh() : m_changed(false)
{
}

void instance_mesh::init_static_atlas(std::shared_ptr<GraphicsComponent> graph, std::string model)
{
	m_graphics = graph;

	m_mesh.add_instanced_base_to_mesh_static_atlas(model);
	m_graphics->SetNewBuffers(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices());

	m_graphics->_instancedDraw = true;
	m_graphics->_instanced_tex_coord_draw = true;

	m_is_static_atlas = true;
}

void instance_mesh::init_specific_atlas(std::shared_ptr<GraphicsComponent> graph, std::string model, std::string texture, int atlas_surface_area)
{
	m_graphics = graph;

	m_mesh = Mesh(atlas_surface_area);
	m_mesh.add_instanced_base_to_mesh_specific_atlas(model, texture);
	m_graphics->SetNewBuffers(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices());

	m_graphics->_instancedDraw = true;
	m_graphics->_instanced_tex_coord_draw = true;

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
		update_graphics_vbo();
	}

	return m_graphics;
}

void instance_mesh::set_graphics_position(Vector3f pos)
{
	m_graphics->SetPhysics(pos);
}

void instance_mesh::update_graphics_vbo()
{
	m_graphics->set_tex_coord_offsets(m_mesh.get_tex_coords());
	m_graphics->SetupVAO();
	//m_graphics->ResetVBO();
}
