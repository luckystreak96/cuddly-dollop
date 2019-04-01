//
// Created by yanik on 24/03/19.
//

#include "gl_index_buffer.h"

GLIndexBuffer::GLIndexBuffer() {

}

void GLIndexBuffer::bind() {
    // Bind IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLBuffer<GLuint>::m_id);

    if(GLBuffer<GLuint>::m_id == 0)
        std::cout << "BUFFER ID IS 0 - IS NOT BINDING" << std::endl;
}

void GLIndexBuffer::bind_and_update() {
    BaseGLBuffer::bind_and_update();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLBuffer<GLuint>::m_buffer.size() * sizeof(GLuint), &GLBuffer<GLuint>::m_buffer[0], GL_STATIC_DRAW);


}

void GLIndexBuffer::bind_for_draw() {
    BaseGLBuffer::bind_for_draw();
}

GLsizei GLIndexBuffer::get_index_count() {
    return (GLsizei)m_buffer.size();
}
