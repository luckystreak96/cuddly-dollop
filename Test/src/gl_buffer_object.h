//
// Created by yanik on 24/03/19.
//

#ifndef PROJECT_GL_BUFFER_OBJECT_H
#define PROJECT_GL_BUFFER_OBJECT_H

#include <GL/glew.h>//keep this up here to avoid "included gl.h before glew.h" error

// This class exists to encapsulate the GLBuffer object creation/destruction
class GLBufferObject
{
public:
    GLBufferObject();

    // These "big five" are necessary to avoid garbage collecting the buffer object
    virtual ~GLBufferObject();
    GLBufferObject(GLBufferObject&& other);
    GLBufferObject(const GLBufferObject& other);
    GLBufferObject& operator=(const GLBufferObject& other);
    GLBufferObject& operator=(GLBufferObject&& other);

    virtual void gen_id();

protected:
    GLuint m_id;
};

#endif //PROJECT_GL_BUFFER_OBJECT_H
