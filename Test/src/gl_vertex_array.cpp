//
// Created by yanik on 24/03/19.
//

#include "gl_vertex_array.h"

GLVertexArray::GLVertexArray() {

}

void GLVertexArray::setup_VAO() {
    // Bind VAO
    glBindVertexArray(m_id);

    for(auto& x : m_buffers)
        x->bind_for_draw();

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
