//
// Created by yanik on 26/03/19.
//

#include "effect_buffer.h"
#include "gl_index_buffer.h"

EffectBuffer::EffectBuffer() {
    m_buffers = std::shared_ptr<GLVertexArray>(new GLVertexArray());
    m_buffers->gen_id();
}

void EffectBuffer::set_vertex_array_buffer(std::shared_ptr<GLVertexArray> vab) {
    m_buffers = vab;
    m_buffers->setup_VAO();
}

GLVertexArray* EffectBuffer::get_vab() {
    return m_buffers.get();
}

std::vector<GLuint> *EffectBuffer::update_index_buffer() {
    BaseGLBuffer* buffer = m_buffers->get_buffer(BT_Index);
    return dynamic_cast<GLIndexBuffer*>(buffer)->get_and_modify_buffer();
}

std::vector<Vector3f> *EffectBuffer::update_vector3f_buffer(BufferTypes bt) {
    if(bt < BT_Color)
        return nullptr;

    BaseGLBuffer* buffer = m_buffers->get_buffer(bt);
    return dynamic_cast<GLVertexBuffer<Vector3f>*>(buffer)->get_and_modify_buffer();
}

std::vector<Vertex> *EffectBuffer::update_vertex_buffer() {
    BaseGLBuffer* buffer = m_buffers->get_buffer(BT_Vertices);
    return dynamic_cast<GLVertexBuffer<Vertex>*>(buffer)->get_and_modify_buffer();
}

void EffectBuffer::bind() {
    m_buffers->bind();
}

void EffectBuffer::unbind() {
    m_buffers->unbind();
}

bool EffectBuffer::has_buffer(BufferTypes bufferTypes) {
    return m_buffers->has_buffer(bufferTypes);
}

std::vector<Mat4f> *EffectBuffer::update_model_buffer() {
    BaseGLBuffer* buffer = m_buffers->get_buffer(BT_Model);
    return dynamic_cast<GLVertexBuffer<Mat4f>*>(buffer)->get_and_modify_buffer();
}

std::vector<Vector2f> *EffectBuffer::update_tex_coord_buffer() {
    BaseGLBuffer* buffer = m_buffers->get_buffer(BT_TextureCoordinates);
    return dynamic_cast<GLVertexBuffer<Vector2f>*>(buffer)->get_and_modify_buffer();
}

GLVertexBuffer<Mat4f>* EffectBuffer::get_models_gl_buffer() {
    BaseGLBuffer* buffer = m_buffers->get_buffer(BT_Model);
    return dynamic_cast<GLVertexBuffer<Mat4f>*>(buffer);
}

GLIndexBuffer *EffectBuffer::get_index_gl_buffer() {
    BaseGLBuffer* buffer = m_buffers->get_buffer(BT_Index);
    return dynamic_cast<GLIndexBuffer*>(buffer);
}

unsigned long EffectBuffer::get_instance_count() {
    unsigned long max = 0;
    for(auto& x : *m_buffers->get_all_buffers())
        max = std::max(x.second->get_instance_count(), max);

    return max;
}

GLsizei EffectBuffer::get_index_count() {
    return get_index_gl_buffer()->get_index_count();
}

unsigned long EffectBuffer::num_buffers() {
    return m_buffers->get_all_buffers()->size();
}

bool EffectBuffer::GLErrorCheck()
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cout << glewGetErrorString(err) << std::endl;
        return true;
    }

    return false;
}

GLVertexBuffer<Vector2f> *EffectBuffer::get_tex_gl_buffer() {
    BaseGLBuffer* buffer = m_buffers->get_buffer(BT_TextureCoordinates);
    return dynamic_cast<GLVertexBuffer<Vector2f>*>(buffer);
}

