//
// Created by yanik on 24/03/19.
//

#ifndef PROJECT_GL_BUFFER_H
#define PROJECT_GL_BUFFER_H

#include <vector>
#include "gl_buffer_object.h"

// Exists for the sake of polymorphism
class BaseGLBuffer
{
public:
    BaseGLBuffer() : m_needs_update(true), m_instance_count(0) {}

    virtual void bind() {}
    virtual void bind_and_update() { m_needs_update = false; this->bind(); }
    virtual void bind_for_draw() {
        if(m_needs_update)
            this->bind_and_update();
        else
            this->bind();
    }

    bool needs_update() {
        return m_needs_update;
    }

    unsigned long get_instance_count() {
        return m_instance_count;
    }

protected:
    bool m_needs_update;
    unsigned long m_instance_count;
};

// This class encapsulates the templating of the buffer list itself
template <class BufferClass>
class GLBuffer : public BaseGLBuffer, public GLBufferObject
{
public:
    GLBuffer() = default;

    // These "big five" are necessary to properly handle buffer data
    GLBuffer(GLBuffer&& other);
    GLBuffer(const GLBuffer& other);
    GLBuffer& operator=(const GLBuffer& other);
    GLBuffer& operator=(GLBuffer&& other);

    std::vector<BufferClass>* get_and_modify_buffer();

    unsigned long size();

protected:
    std::vector<BufferClass> m_buffer;
};

template<class BufferClass>
GLBuffer<BufferClass>::GLBuffer(GLBuffer &&other) : GLBufferObject(other) {
    std::swap(m_buffer, other.m_buffer);
}

template<class BufferClass>
GLBuffer<BufferClass>::GLBuffer(const GLBuffer &other) : GLBufferObject(other) {
    // Deep copy of all elements
    m_buffer = other.m_buffer;
}

template<class BufferClass>
GLBuffer<BufferClass> &GLBuffer<BufferClass>::operator=(const GLBuffer &other) {
    // Deep copy of all elements
    m_buffer = other.m_buffer;

    return *this;
}

template<class BufferClass>
GLBuffer<BufferClass> &GLBuffer<BufferClass>::operator=(GLBuffer &&other) {
    std::swap(m_buffer, other.m_buffer);
    return *this;
}

template<class BufferClass>
std::vector<BufferClass> *GLBuffer<BufferClass>::get_and_modify_buffer() {
    if(m_id == 0)
        this->gen_id();
    m_needs_update = true;
    return &m_buffer;
}

template<class BufferClass>
unsigned long GLBuffer<BufferClass>::size() {
    return m_buffer.size();
}

#endif //PROJECT_GL_BUFFER_H
