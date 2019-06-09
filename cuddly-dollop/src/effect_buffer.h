//
// Created by yanik on 26/03/19.
//

#ifndef PROJECT_EFFECT_BUFFER_H
#define PROJECT_EFFECT_BUFFER_H

#include <memory>

#include "gl_vertex_array.h"
#include "effectManager.h"
#include "gl_vertex_buffer.h"
#include "gl_index_buffer.h"

// Abstracts the vertex array and operations to it
class EffectBuffer
{
public:
    EffectBuffer();

    void set_vertex_array_buffer(std::shared_ptr<GLVertexArray> vab);
    GLVertexArray* get_vab();

    std::vector<GLuint>* update_index_buffer();
    std::vector<Vertex>* update_vertex_buffer();
    std::vector<Mat4f>* update_model_buffer();
    std::vector<Vector2f>* update_tex_coord_buffer();
    std::vector<Vector3f>* update_vector3f_buffer(BufferTypes bt);

    GLVertexBuffer<Mat4f>* get_models_gl_buffer();
    GLVertexBuffer<Vector2f>* get_tex_gl_buffer();
    GLIndexBuffer* get_index_gl_buffer();

    unsigned long get_instance_count();
    GLsizei get_index_count();

    bool has_buffer(BufferTypes bufferTypes);
    unsigned long num_buffers();

    static bool GLErrorCheck();

    void bind();
    void unbind();

protected:
    std::shared_ptr<GLVertexArray> m_buffers;
};

#endif //PROJECT_EFFECT_BUFFER_H
