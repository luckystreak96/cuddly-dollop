//
// Created by yanik on 26/03/19.
//

#ifndef PROJECT_EFFECT_BUFFER_H
#define PROJECT_EFFECT_BUFFER_H

#include <memory>

#include "gl_vertex_array.h"
#include "effectManager.h"
#include "gl_vertex_buffer.h"

class EffectBuffer
{
public:
    EffectBuffer();

    void set_vertex_array_buffer(std::shared_ptr<GLVertexArray> vab);
    GLVertexArray* get_vab();

    std::vector<GLuint>* update_index_buffer();
    std::vector<Vector3f>* update_vector3f_buffer(BufferTypes bt);
    std::vector<Vertex>* update_vertex_buffer();

    bool has_buffer(BufferTypes bufferTypes);

    void bind();

protected:
    std::shared_ptr<GLVertexArray> m_buffers;
};

#endif //PROJECT_EFFECT_BUFFER_H
