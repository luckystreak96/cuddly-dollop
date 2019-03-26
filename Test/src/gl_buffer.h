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
    BaseGLBuffer() : m_needs_update(true) {}

    virtual void bind() {}
    virtual void bind_and_update() { m_needs_update = false; this->bind(); }
    // MAY BE BAD - NOT SURE IF CHILD METHODS WILL BE CALLED FROM HERE
    virtual void bind_for_draw() {
        if(m_needs_update)
            this->bind_and_update();
        else
            this->bind();
    }

protected:
    bool m_needs_update;
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

protected:
    std::vector<BufferClass> m_buffer;
};

template<class BufferClass>
GLBuffer<BufferClass>::GLBuffer(GLBuffer &&other) {
    std::swap(m_buffer, other.m_buffer);
}

template<class BufferClass>
GLBuffer<BufferClass>::GLBuffer(const GLBuffer &other) {
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

#endif //PROJECT_GL_BUFFER_H
