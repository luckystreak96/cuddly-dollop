//
// Created by yanik on 24/03/19.
//

#ifndef PROJECT_GL_VERTEX_BUFFER_H
#define PROJECT_GL_VERTEX_BUFFER_H

#include "gl_buffer.h"
#include "math.h"

#include <iostream>

template <class BufferClass>
class GLVertexBuffer : public GLBuffer<BufferClass>
{
public:
    GLVertexBuffer();
    GLVertexBuffer(GLuint pos, std::vector<GLint> offsets);
    GLVertexBuffer(std::vector<BufferClass> values);
    void set_position(GLuint pos);
    void set_size_offsets(std::vector<GLint> offsets);
    void bind();
    void bind_and_update();
    void bind_for_draw();

    void set_instanced_draw(bool instanced);

protected:
    std::vector<GLint> m_size_offsets;
    GLuint m_position;
    bool m_instanced_draw;
};

template<class BufferClass>
GLVertexBuffer<BufferClass>::GLVertexBuffer() : m_position(0), m_instanced_draw(false) {
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
    BaseGLBuffer::bind_and_update();
    glBufferData(GL_ARRAY_BUFFER, GLBuffer<BufferClass>::m_buffer.size() * sizeof(BufferClass), &GLBuffer<BufferClass>::m_buffer[0], GL_STATIC_DRAW);
}

template<class BufferClass>
void GLVertexBuffer<BufferClass>::bind_for_draw() {
    BaseGLBuffer::bind_for_draw();

    // Set vertex attribs
    int acc = 0;
    for(int i = 0; i < m_size_offsets.size(); i++)
    {
        glEnableVertexAttribArray(m_position + i);
        glVertexAttribPointer(m_position + i, m_size_offsets[i], GL_FLOAT, GL_FALSE, sizeof(BufferClass), (const GLvoid*)acc);//vertex position
        glVertexAttribDivisor(m_position + i, m_instanced_draw ? 1 : 0);
        acc += m_size_offsets[i];
    }

    glVertexAttribDivisor(m_position, m_instanced_draw ? 1 : 0);
}

template<class BufferClass>
void GLVertexBuffer<BufferClass>::set_instanced_draw(bool instanced) {
    m_instanced_draw = instanced;
}

template<class BufferClass>
GLVertexBuffer<BufferClass>::GLVertexBuffer(std::vector<BufferClass> values) {
    GLBuffer<BufferClass>::m_buffer = values;
    this->gen_id();
}

#endif //PROJECT_GL_VERTEX_BUFFER_H
