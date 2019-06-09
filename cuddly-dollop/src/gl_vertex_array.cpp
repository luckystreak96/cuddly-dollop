//
// Created by yanik on 24/03/19.
//

#include "gl_vertex_array.h"

#include "gl_buffer_factory.h"

GLVertexArray::GLVertexArray() : m_buffers(std::map<BufferTypes, std::shared_ptr<BaseGLBuffer>>()), m_prev_buffer_count(0) {

}

void GLVertexArray::setup_VAO() {
    if(!m_id)
        gen_id();

    // Bind VAO
    glBindVertexArray(m_id);

    for(auto& x : m_buffers)
        x.second->bind_for_draw();

    glBindVertexArray(0);

    m_prev_buffer_count = m_buffers.size();
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
    if(!m_buffers.count(bt))
        GLBufferFactory::add_empty_buffer(this, bt);

    return m_buffers[bt].get();
}

void GLVertexArray::bind() {
    if(m_id == 0 || m_prev_buffer_count != m_buffers.size())
        setup_VAO();

//    if (!m_buffers.count(BT_TextureCoordinates))
//    {
//        unsigned long target_size = dynamic_cast<GLVertexBuffer<Vertex>*>(m_buffers[BT_Vertices].get())->size();
//
//        GLBufferFactory::add_empty_buffer(this, BT_TextureCoordinates);
//        auto b = dynamic_cast<GLVertexBuffer<Vector2f>*>(m_buffers[BT_TextureCoordinates].get());
//        while(b->size() < target_size)
//            b->get_and_modify_buffer()->push_back(Vector2f());
//    }


    for(auto& x : m_buffers)
        if(x.second->needs_update())
            x.second->bind_and_update();

    // Bind VAO (which binds vbo + IBO)
    glBindVertexArray(m_id);
}

void GLVertexArray::unbind() {
    glBindVertexArray(0);
}

bool GLVertexArray::has_buffer(BufferTypes bufferTypes) {
    return m_buffers.count(bufferTypes) > 0;
}

bool GLVertexArray::is_loaded() {
    return m_buffers.size() >= 2;
}

std::map<BufferTypes, std::shared_ptr<BaseGLBuffer>>* GLVertexArray::get_all_buffers() {
    return &m_buffers;
}
