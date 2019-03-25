//
// Created by yanik on 24/03/19.
//

#ifndef PROJECT_GL_VERTEX_BUFFER_H
#define PROJECT_GL_VERTEX_BUFFER_H

#include "gl_buffer.h"

#include <iostream>

template <class BufferClass>
class GLVertexBuffer : public GLBuffer<BufferClass>
{
public:
    GLVertexBuffer();
    GLVertexBuffer(GLuint pos, std::vector<GLint> offsets);
    void set_position(GLuint pos);
    void set_size_offsets(std::vector<GLint> offsets);
    void bind();
    void bind_and_update();
    void bind_for_draw();

protected:
    std::vector<GLint> m_size_offsets;
    GLuint m_position;
    bool m_needs_update;
};

template<class BufferClass>
GLVertexBuffer<BufferClass>::GLVertexBuffer() : m_position(0), m_needs_update(true) {
}

template<class BufferClass>
void GLVertexBuffer<BufferClass>::set_position(GLuint pos) {
    m_position = pos;
}

template<class BufferClass>
void GLVertexBuffer<BufferClass>::set_size_offsets(std::vector<GLint> offsets) {
    m_size_offsets = offsets;
}

template<class BufferClass>
GLVertexBuffer<BufferClass>::GLVertexBuffer(GLuint pos, std::vector<GLint> offsets) {
    m_position = pos;
    m_size_offsets = offsets;
    m_needs_update = true;
}

template<class BufferClass>
void GLVertexBuffer<BufferClass>::bind() {
    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, GLBuffer<BufferClass>::m_id);

    if(GLBuffer<BufferClass>::m_id == 0)
        std::cout << "BUFFER ID IS 0 - IS NOT BINDING" << std::endl;
}

template<class BufferClass>
void GLVertexBuffer<BufferClass>::bind_and_update() {
    bind();
    glBufferData(GL_ARRAY_BUFFER, GLBuffer<BufferClass>::_buffer.size() * sizeof(BufferClass), &GLBuffer<BufferClass>::_buffer[0], GL_STATIC_DRAW);
}

template<class BufferClass>
void GLVertexBuffer<BufferClass>::bind_for_draw() {
    if(m_needs_update)
        bind_and_update();
    else
        bind();

    // Set vertex attribs
    int acc = 0;
    for(int i = 0; i < m_size_offsets.size(); i++)
    {
        glEnableVertexAttribArray(m_position + i);
        glVertexAttribPointer(m_position + i, m_size_offsets[i], GL_FLOAT, GL_FALSE, sizeof(BufferClass), (const GLvoid*)acc);//vertex position
        acc += m_size_offsets[i];
    }
}

#endif //PROJECT_GL_VERTEX_BUFFER_H
