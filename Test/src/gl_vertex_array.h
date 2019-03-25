//
// Created by yanik on 24/03/19.
//

#ifndef PROJECT_GL_VERTEX_ARRAY_H
#define PROJECT_GL_VERTEX_ARRAY_H

#include <vector>
#include <memory>

#include "gl_buffer.h"

class GLVertexArray : public GLBufferObject
{
public:
    GLVertexArray();
    ~GLVertexArray();

    void setup_VAO();
    void gen_id();

private:
    std::vector<std::shared_ptr<BaseGLBuffer>> m_buffers;
};

#endif //PROJECT_GL_VERTEX_ARRAY_H
