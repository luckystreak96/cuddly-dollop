//
// Created by yanik on 29/03/19.
//

#include "graphics_component.h"

#include "gl_buffer_factory.h"

float GraphicsComponent::_persRotation = -0.4f;
float GraphicsComponent::_orthoRotation = -0.0325f;

float GraphicsComponent::GetProjectionRotation()
{
    return Transformation::perspectiveOrtho ? _orthoRotation : _persRotation;
}

GraphicsComponent::GraphicsComponent(std::string modelName, std::string texPath) : m_modelName(modelName)
{
    m_texture = texPath;
    set_texture(m_texture);

    m_modelName = modelName;
    GLBufferFactory::set_vert_ind_buffers(&m_buffers, m_modelName);
}

GraphicsComponent::~GraphicsComponent()
{
}

void GraphicsComponent::Update()
{
    if (UpdateTranslation() || OrthoProjInfo::GetRegularInstance().changed)
        UpdateMModels();
}

void GraphicsComponent::UpdateMModels()
{
    if(m_buffers.update_model_buffer()->empty())
        InsertMModels(m_modelMat);
    else
        InsertMModels(m_modelMat, 0);
}

bool GraphicsComponent::UpdateTranslation()
{
    Vector3f translation;
    Vector3f offset;

    float size = OrthoProjInfo::GetRegularInstance().Size;
    if (m_modelName == "CENTERED_TILE")
        offset = Vector3f(0.5f * size, 0.5f * size, 0.0f);
    //else if (m_modelName == "SCREEN")
    //	translation = m_position/* + Vector3f(0.5f, 0.5f, 0)*/;
    //else
    //	translation = m_position;

    translation = m_position + offset;

    if (translation == m_modelMat.GetTranslation())
        return false;

    m_modelMat.SetTranslation(translation);

    return true;
}

bool GraphicsComponent::will_not_draw()
{
    return (!m_texture_loaded || !mustDraw || m_modelName == "NONE");
}


void GraphicsComponent::Draw(bool withTex)
{
    // Must be drawn?
    if (will_not_draw())
        return;

    // Bind texture
    if (withTex)
    {
        //Texture* tex = ResourceManager::GetInstance().get_texture(m_texture);
        if (m_tex_ptr)
            m_tex_ptr->Bind(GL_TEXTURE0);
    }

    // Binds VertexArray (which includes all other buffers + updates them if necessary
    m_buffers.bind();

    // Draw
    if (m_buffers.get_instance_count() > 0)
    {
        glDrawElementsInstanced(GL_TRIANGLES, m_buffers.get_index_count(), GL_UNSIGNED_INT, nullptr, (int)m_buffers.get_instance_count());
    }
    else
    {
        glDrawElements(GL_TRIANGLES, m_buffers.get_index_count(), GL_UNSIGNED_INT, nullptr);
    }

    //Unbind VAO
    m_buffers.unbind();
}

// Sets the color of all vertices to change the color of the sprite
void GraphicsComponent::SetColorAll(Vector3f color, float alpha)
{
    auto verts = m_buffers.update_vertex_buffer();
    if (verts->size() && verts->at(0).color == color && verts->at(0).alpha == alpha)
        return;

    Vertex::SetColorAll(verts->begin(), verts->end(), color, alpha);
}

// Adds 4 model matrices to MModels (does not clear MModels first)
void GraphicsComponent::InsertMModels(Transformation& t)
{
    InsertMModels(t, -1);
}

// Updates 4 model matrices to MModels instead of appending
void GraphicsComponent::InsertMModels(Transformation& t, int position)
{
    Vector3f reminder = t.GetTranslation();
    Vector3f temp = t.GetTranslation();
    if (!Transformation::perspectiveOrtho)
        temp.z *= OrthoProjInfo::GetRegularInstance().Size;
    else
        temp.z *= 1.3f;
    t.SetTranslation(temp);

    if(position == -1)
        m_buffers.get_models_gl_buffer()->append_values(t.GetWorldTrans());
    else
        m_buffers.get_models_gl_buffer()->update_values(t.GetWorldTrans(), position);

    t.SetTranslation(reminder);
}

// Updates 4 model matrices to MModels instead of appending + uses a pre-computed matrix
void GraphicsComponent::InsertMModels(Mat4f& mat, int position)
{
    float size = OrthoProjInfo::GetRegularInstance().Size;
    mat.m[0][3] *= size;
    mat.m[1][3] *= size;
    mat.m[2][3] *= size;

    m_buffers.get_models_gl_buffer()->update_values(mat, position);
}

EffectBuffer *GraphicsComponent::get_buffers() {
    return &m_buffers;
}

void GraphicsComponent::set_scale(Vector3f scale) {
    m_modelMat.SetScale(scale);
    UpdateMModels();
}


