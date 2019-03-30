//
// Created by yanik on 29/03/19.
//

#include "catch.hpp"

#include "../src/gl_buffer_factory.h"

TEST_CASE("EffectBuffer initialization", "[EffectBuffer]")
{
    EffectBuffer eb = EffectBuffer();
    GLBufferFactory::set_vert_ind_buffers(&eb, "CENTERED_TILE");

    eb.bind();

    SECTION("VertexArray is generated properly"){
        REQUIRE(glIsVertexArray(eb.get_vab()->get_id()) == GL_TRUE);
    }
}
