//
// Created by yanik on 24/03/19.
//

#include "gl_vertex_array.h"

GLVertexArray::GLVertexArray() : m_buffers(std::map<BufferTypes, std::shared_ptr<BaseGLBuffer>>()) {

}

void GLVertexArray::setup_VAO() {
    if(!m_id)
        gen_id();

    // Bind VAO
    glBindVertexArray(m_id);

    for(auto& x : m_buffers)
        x.second->bind_for_draw();

    glBindVertexArray(0);
}

void GLVertexArray::gen_id() {
    if(m_id == 0)
        glGenVertexArrays(1, &m_id);
}

// This destructor will get called first -- setting ID to 0 will prevent other dtor from deleting unwanted buffer
GLVertexArray::~GLVertexArray() {
    if (m_id != 0)
        glDeleteVertexArrays(1, &m_id);

    m_id = 0;
}

void GLVertexArray::set_all_buffers(std::map<BufferTypes, std::shared_ptr<BaseGLBuffer>> buffers) {
    m_buffers = buffers;
}

void GLVertexArray::set_buffer(BufferTypes bt, std::shared_ptr<BaseGLBuffer> buffer) {
    m_buffers[bt] = buffer;
}

BaseGLBuffer *GLVertexArray::get_buffer(BufferTypes bt) {
    if(m_buffers.count(bt))
        return m_buffers[bt].get();

    return nullptr;
}

void GLVertexArray::bind() {
    // Bind VAO (which binds vbo + IBO)
    glBindVertexArray(m_id);
}

bool GLVertexArray::has_buffer(BufferTypes bufferTypes) {
    return m_buffers.count(bufferTypes) > 0;
}
