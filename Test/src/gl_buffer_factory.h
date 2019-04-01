//
// Created by yanik on 25/03/19.
//

#ifndef PROJECT_GL_BUFFER_FACTORY_H
#define PROJECT_GL_BUFFER_FACTORY_H

#include <memory>
#include "gl_buffer.h"
#include "gl_vertex_array.h"
#include "effect_buffer.h"

class GLBufferFactory
{
public:
    static std::shared_ptr<BaseGLBuffer> create_index_buffer(std::vector<GLuint> indices = std::vector<GLuint>());
    static std::shared_ptr<GLVertexArray> create_vertex_array(std::map<BufferTypes, std::shared_ptr<BaseGLBuffer>> buffers);
    static std::shared_ptr<EffectBuffer> create_effect_buffer(std::string model_name);
    static void set_vert_ind_buffers(EffectBuffer* buffer, std::string name);

//    template <class BufferClass>
//    static void add_empty_buffer(GLVertexArray* vab, BufferTypes bt);
    static void add_empty_buffer(GLVertexArray *vab, BufferTypes bt);

    template <class BufferClass>
    static std::shared_ptr<BaseGLBuffer> create_vertex_buffer(std::vector<BufferClass> vertices, std::vector<GLuint> offsets, GLuint position, bool instanced);
};

#endif //PROJECT_GL_BUFFER_FACTORY_H
