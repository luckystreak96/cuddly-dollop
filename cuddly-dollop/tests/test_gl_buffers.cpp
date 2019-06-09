//
// Created by yanik on 28/03/19.
//

#include "catch.hpp"

#include "../src/gl_vertex_array.h"
#include "../src/gl_vertex_buffer.h"

TEST_CASE("GLVertexArray Buffer Generation", "[VertexArray]")
{
    // Init
    GLVertexBuffer<int>* buffer;
    GLuint bid;
    std::vector<int> values = {0, 1, 2};

    SECTION("Default m_id = 0")
    {
        buffer = new GLVertexBuffer<int>();
        REQUIRE(buffer->get_id() == 0);

        delete buffer;
    }

    SECTION("Destructor clears id") {
        buffer = new GLVertexBuffer<int>();
        buffer->gen_id();
        *buffer->get_and_modify_buffer() = values;
        buffer->bind_and_update();
        bid = buffer->get_id();
        REQUIRE(glIsBuffer(bid) == GL_TRUE);
        delete buffer;

        REQUIRE(glIsBuffer(bid) == GL_FALSE);
    }

    SECTION("Can't generate 2 buffers / bind_for_draw notices when the buffer isn't clean") {
        buffer = new GLVertexBuffer<int>();
        buffer->gen_id();
        bid = buffer->get_id();
        *buffer->get_and_modify_buffer() = values;
        buffer->bind_for_draw();
        REQUIRE(glIsBuffer(bid) == GL_TRUE);
        bid = buffer->get_id();

        buffer->gen_id();
        GLuint bid2 = buffer->get_id();
        REQUIRE(bid == bid2);
        delete buffer;
    }

    SECTION("Copy constructor") {
        buffer = new GLVertexBuffer<int>();
        buffer->gen_id();
        bid = buffer->get_id();
        *buffer->get_and_modify_buffer() = values;
        buffer->bind_for_draw();

        GLVertexBuffer<int>* buffer2 = new GLVertexBuffer<int>(*buffer);
        GLuint bid2 = buffer2->get_id();
        buffer2->gen_id();
        buffer2->bind_for_draw();

        REQUIRE(bid2 != bid);
        REQUIRE(glIsBuffer(bid2) == GL_TRUE);

        delete buffer;
        delete buffer2;
    }

    SECTION("Move semantics") {
        int counter = GLBufferObject::get_id_counter();
        GLVertexBuffer<int>* buffer2 = new GLVertexBuffer<int>(GLVertexBuffer<int>(values));
        buffer2->bind_for_draw();
        GLuint bid2 = buffer2->get_id();
        int counter2 = GLBufferObject::get_id_counter();

        REQUIRE(counter2 - counter == 1);
        REQUIRE(glIsBuffer(bid2) == GL_TRUE);
    }
}
