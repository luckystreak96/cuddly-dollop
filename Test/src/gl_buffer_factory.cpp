//
// Created by yanik on 25/03/19.
//

#include "gl_buffer_factory.h"

#include "gl_index_buffer.h"
#include "gl_vertex_buffer.h"
#include "model.h"

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
GLBufferFactory::create_vertex_array(std::map<BufferTypes, std::shared_ptr<BaseGLBuffer>> buffers) {
    std::shared_ptr<GLVertexArray> result = std::shared_ptr<GLVertexArray>(new GLVertexArray());
    result->gen_id();
    result->set_all_buffers(buffers);
    result->setup_VAO();
    return result;
}

std::shared_ptr<EffectBuffer> GLBufferFactory::create_effect_buffer(std::string model_name) {
    std::shared_ptr<EffectBuffer> result = std::shared_ptr<EffectBuffer>(new EffectBuffer());
    set_vert_ind_buffers(result.get(), model_name);
}

void GLBufferFactory::set_vert_ind_buffers(EffectBuffer *buffer, std::string name) {
    // Create empty buffers to fetch and modify
    add_empty_buffer<GLuint>(buffer->get_vab(), BT_Index);
    add_empty_buffer<Vertex>(buffer->get_vab(), BT_Vertices);

    // Fill the buffers with the data
    std::vector<Vertex>* verts = buffer->update_vertex_buffer();
    std::vector<GLuint>* index_buffer = buffer->update_index_buffer();

    Model::GetInstance().loadModel(name);

    // Vertices
    auto vertices = std::vector<Vertex>(Model::GetInstance().getVertexVertices());
    float size = OrthoProjInfo::GetRegularInstance().Size;
    for(auto& x : vertices)
    {
        x.vertex.x *= size;
        x.vertex.y *= size;
    }
    verts->swap(vertices);

    // Indices
    std::vector<GLuint> indices = std::vector<GLuint>(Model::GetInstance().getIndices());
    index_buffer->swap(indices);

    // Set the correct offsets
    BaseGLBuffer* temp = buffer->get_vab()->get_buffer(BT_Vertices);
    auto vert_buf = dynamic_cast<GLVertexBuffer<Vertex>*>(temp);
    vert_buf->set_size_offsets(std::vector<GLint> {3, 2, 3, 1});

}

template<class BufferClass>
void GLBufferFactory::add_empty_buffer(GLVertexArray *vab, BufferTypes bt) {
    std::shared_ptr<BaseGLBuffer> result;

    // Create empty buffers depending on specification
    if(bt == BT_Index)
        result = create_index_buffer();
    else
        result = std::shared_ptr<BaseGLBuffer>(new GLVertexBuffer<BufferClass>());

    // Sets the empty buffer
    vab->set_buffer(bt, result);
}
