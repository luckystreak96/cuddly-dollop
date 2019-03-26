//
// Created by yanik on 25/03/19.
//

#ifndef PROJECT_GL_BUFFER_FACTORY_H
#define PROJECT_GL_BUFFER_FACTORY_H

#include <memory>
#include "gl_buffer.h"
#include "gl_vertex_array.h"

class GLBufferFactory
{
public:
    static std::shared_ptr<BaseGLBuffer> create_index_buffer(std::vector<GLuint> indices);
    static std::shared_ptr<GLVertexArray> create_vertex_array(std::vector<std::shared_ptr<BaseGLBuffer>> buffers);

    template <class BufferClass>
    static std::shared_ptr<BaseGLBuffer> create_vertex_buffer(std::vector<BufferClass> vertices, std::vector<GLuint> offsets, GLuint position, bool instanced);
};

#endif //PROJECT_GL_BUFFER_FACTORY_H
