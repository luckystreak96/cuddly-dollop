//
// Created by yanik on 25/03/19.
//

#include "gl_buffer_factory.h"

#include "gl_index_buffer.h"
#include "gl_vertex_buffer.h"

std::shared_ptr<BaseGLBuffer> GLBufferFactory::create_index_buffer(std::vector<GLuint> indices) {
    std::shared_ptr<GLIndexBuffer> result = std::shared_ptr<GLIndexBuffer>(new GLIndexBuffer());
    *result->get_and_modify_buffer() = indices;
    return result;
}

template<class BufferClass>
std::shared_ptr<BaseGLBuffer>
GLBufferFactory::create_vertex_buffer(std::vector<BufferClass> vertices, std::vector<GLuint> offsets, GLuint position, bool instanced) {
    std::shared_ptr<GLVertexBuffer<BufferClass>> result = std::shared_ptr<GLVertexBuffer<BufferClass>>(new GLVertexBuffer<BufferClass>(position, offsets));
    *result->get_and_modify_buffer() = vertices;
    *result->set_instanced_draw(instanced);
    return result;
}

std::shared_ptr<GLVertexArray>
GLBufferFactory::create_vertex_array(std::vector<std::shared_ptr<BaseGLBuffer>> buffers) {
    std::shared_ptr<GLVertexArray> result = std::shared_ptr<GLVertexArray>(new GLVertexArray());
    result->gen_id();
    result->set_buffers(buffers);
    result->setup_VAO();
    return result;
}
