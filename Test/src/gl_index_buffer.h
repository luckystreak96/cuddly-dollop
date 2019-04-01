//
// Created by yanik on 24/03/19.
//

#ifndef PROJECT_GL_INDEX_BUFFER_H
#define PROJECT_GL_INDEX_BUFFER_H

#include "gl_buffer.h"

#include <iostream>

class GLIndexBuffer : public GLBuffer<GLuint>
{
public:
    GLIndexBuffer();
    void bind();
    void bind_and_update();
    void bind_for_draw();

    GLsizei get_index_count();
};

#endif //PROJECT_GL_INDEX_BUFFER_H
