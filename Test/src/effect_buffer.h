//
// Created by yanik on 26/03/19.
//

#ifndef PROJECT_EFFECT_BUFFER_H
#define PROJECT_EFFECT_BUFFER_H

#include <memory>

#include "gl_vertex_array.h"

class EffectBuffer
{
public:


protected:
    std::shared_ptr<GLVertexArray> m_buffers;
};

#endif //PROJECT_EFFECT_BUFFER_H
