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
    virtual void bind() {}
    virtual void bind_and_update() {}
    virtual void bind_for_draw() {}
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

public:
    std::vector<BufferClass> _buffer;
};

template<class BufferClass>
GLBuffer<BufferClass>::GLBuffer(GLBuffer &&other) {
    std::swap(_buffer, other._buffer);
}

template<class BufferClass>
GLBuffer<BufferClass>::GLBuffer(const GLBuffer &other) {
    // Deep copy of all elements
    _buffer = other._buffer;
}

template<class BufferClass>
GLBuffer<BufferClass> &GLBuffer<BufferClass>::operator=(const GLBuffer &other) {
    // Deep copy of all elements
    _buffer = other._buffer;

    return *this;
}

template<class BufferClass>
GLBuffer<BufferClass> &GLBuffer<BufferClass>::operator=(GLBuffer &&other) {
    std::swap(_buffer, other._buffer);
    return *this;
}

#endif //PROJECT_GL_BUFFER_H
