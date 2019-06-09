//
// Created by yanik on 24/03/19.
//

#ifndef PROJECT_GL_VERTEX_ARRAY_H
#define PROJECT_GL_VERTEX_ARRAY_H

#include <map>
#include <memory>

#include "gl_buffer.h"

enum BufferTypes {BT_Index, BT_Vertices, BT_TextureCoordinates, BT_Model, BT_Color};

class GLVertexArray : public GLBufferObject
{
public:
    GLVertexArray();
    ~GLVertexArray();

    void setup_VAO();
    void gen_id();

    void bind();
    void unbind();

    bool has_buffer(BufferTypes bufferTypes);
    bool is_loaded();

    void set_all_buffers(std::map<BufferTypes, std::shared_ptr<BaseGLBuffer>> buffers);
    std::map<BufferTypes, std::shared_ptr<BaseGLBuffer>>* get_all_buffers();
    void set_buffer(BufferTypes bt, std::shared_ptr<BaseGLBuffer> buffer);
    BaseGLBuffer* get_buffer(BufferTypes bt);

private:
    std::map<BufferTypes, std::shared_ptr<BaseGLBuffer>> m_buffers;
    int m_prev_buffer_count;
};

#endif //PROJECT_GL_VERTEX_ARRAY_H
