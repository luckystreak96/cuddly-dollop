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

bool EffectBuffer::has_buffer(BufferTypes bufferTypes) {
    return m_buffers->has_buffer(bufferTypes);
}
