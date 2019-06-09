//
// Created by yanik on 24/03/19.
//

#ifndef PROJECT_GL_VERTEX_BUFFER_H
#define PROJECT_GL_VERTEX_BUFFER_H

#include "gl_buffer.h"
#include "math.h"

#include <iostream>

template <class T>
class GLVertexBuffer : public GLBuffer<T>
{
public:
    GLVertexBuffer();
    GLVertexBuffer(GLuint pos, std::vector<GLint> offsets);
    GLVertexBuffer(std::vector<T> values);

    void set_position(GLuint pos);
    void set_size_offsets(std::vector<GLint> offsets);

    void append_values(T& val);
    void update_values(T& val, int position);

    void bind();
    void bind_and_update();
    void bind_for_draw();

    void set_instanced_draw(bool instanced);
    int get_instanced_quantity();

protected:
    std::vector<GLint> m_size_offsets;
    GLuint m_position;
    bool m_instance_buffer;
};

template<class T>
GLVertexBuffer<T>::GLVertexBuffer() : m_position(0), m_instance_buffer(true) {
}

template<class T>
void GLVertexBuffer<T>::set_position(GLuint pos) {
    m_position = pos;
}

template<class T>
void GLVertexBuffer<T>::set_size_offsets(std::vector<GLint> offsets) {
    m_size_offsets = offsets;
}

template<class T>
GLVertexBuffer<T>::GLVertexBuffer(GLuint pos, std::vector<GLint> offsets) {
    m_position = pos;
    m_size_offsets = offsets;
}

template<class T>
void GLVertexBuffer<T>::bind() {
    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, GLBuffer<T>::m_id);

    if(GLBuffer<T>::m_id == 0)
        std::cout << "BUFFER ID IS 0 - IS NOT BINDING" << std::endl;
}

template<class T>
void GLVertexBuffer<T>::bind_and_update() {
    BaseGLBuffer::bind_and_update();
    glBufferData(GL_ARRAY_BUFFER, this->m_buffer.size() * sizeof(T), &this->m_buffer[0], GL_STATIC_DRAW);

//    std::cout << std::string(typeid(T).name()).substr(1) << std::endl;
//    std::cout << this->m_buffer.size() << std::endl;

    if(m_instance_buffer)
        this->m_instance_count = this->m_buffer.size();
    else
        this->m_instance_count = 0;
}

template<class T>
void GLVertexBuffer<T>::bind_for_draw() {
    BaseGLBuffer::bind_for_draw();

    // Set vertex attribs
    int acc = 0;
    for(int i = 0; i < m_size_offsets.size(); i++)
    {
        glEnableVertexAttribArray(m_position + i);
        glVertexAttribPointer(m_position + i, m_size_offsets[i], GL_FLOAT, GL_FALSE, sizeof(T), (const GLvoid*)(sizeof(float) * acc));//vertex position
        if(m_instance_buffer)
            glVertexAttribDivisor(m_position + i, 1);
        acc += m_size_offsets[i];
    }
    //glVertexAttribDivisor(m_position, m_instance_buffer ? 1 : 0);
}

template<class T>
void GLVertexBuffer<T>::set_instanced_draw(bool instanced) {
    m_instance_buffer = instanced;
}

template<class T>
GLVertexBuffer<T>::GLVertexBuffer(std::vector<T> values) {
    this->m_buffer = values;
    this->gen_id();
}

template<class T>
void GLVertexBuffer<T>::append_values(T &val) {
    int num = m_instance_buffer ? 1 : 4;
    this->m_buffer.insert(this->m_buffer.end(), num, val);

    GLBuffer<T>::m_needs_update = true;
}

template<class T>
void GLVertexBuffer<T>::update_values(T &val, int position) {
    int num = m_instance_buffer ? 1 : 4;
    for (int i = 0; i < num; i++)
        this->m_buffer[position * num + i] = val;

    GLBuffer<T>::m_needs_update = true;
}

template<class T>
int GLVertexBuffer<T>::get_instanced_quantity() {
    if(m_instance_buffer)
        return this->m_buffer.size();

    return 0;
}

#endif //PROJECT_GL_VERTEX_BUFFER_H
