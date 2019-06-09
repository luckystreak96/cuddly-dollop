//
// Created by yanik on 24/03/19.
//

#include <iostream>
#include "gl_buffer_object.h"

int GLBufferObject::m_id_counter = 0;

GLBufferObject::GLBufferObject() : m_id(0) {

}

GLBufferObject::~GLBufferObject() {
    if (m_id != 0)
        glDeleteBuffers(1, &m_id);
}

GLBufferObject::GLBufferObject(GLBufferObject &&other) {
    m_id = other.m_id;
    other.m_id = 0;
}

GLBufferObject::GLBufferObject(const GLBufferObject &other) : m_id(0) {
    gen_id();
}

GLBufferObject &GLBufferObject::operator=(const GLBufferObject &other) {
    gen_id();
    return *this;
}

GLBufferObject &GLBufferObject::operator=(GLBufferObject &&other) {
    m_id = other.m_id;
    other.m_id = 0;

    return *this;
}

void GLBufferObject::gen_id() {
    if(m_id == 0) {
        glGenBuffers(1, &m_id);
        m_id_counter++;
    }
}

GLuint GLBufferObject::get_id() {
    return m_id;
}

int GLBufferObject::get_id_counter() {
    return m_id_counter;
}

