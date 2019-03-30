//
// Created by yanik on 24/03/19.
//

#ifndef PROJECT_GL_VERTEX_ARRAY_H
#define PROJECT_GL_VERTEX_ARRAY_H

#include <map>
#include <memory>

#include "gl_buffer.h"

enum BufferTypes {BT_Index, BT_Vertices, BT_TextureCoordinates, BT_Color};

class GLVertexArray : public GLBufferObject
{
public:
    GLVertexArray();
    ~GLVertexArray();

    void setup_VAO();
    void gen_id();
    void bind();
    bool has_buffer(BufferTypes bufferTypes);

    void set_all_buffers(std::map<BufferTypes, std::shared_ptr<BaseGLBuffer>> buffers);
    void set_buffer(BufferTypes bt, std::shared_ptr<BaseGLBuffer> buffer);
    BaseGLBuffer* get_buffer(BufferTypes bt);

private:
    std::map<BufferTypes, std::shared_ptr<BaseGLBuffer>> m_buffers;
};

#endif //PROJECT_GL_VERTEX_ARRAY_H
