//
// Created by yanik on 29/03/19.
//

#include "catch.hpp"

#include "../src/graphics_component.h"
#include "../src/effectManager.h"

TEST_CASE("GraphicsComponent Construction", "[GraphicsComponent]"){
    EffectBuffer::GLErrorCheck();

    GraphicsComponent graphicsComponent = GraphicsComponent();

    SECTION("Constructor creates simple graphics component"){
        REQUIRE(!EffectBuffer::GLErrorCheck());
        REQUIRE(graphicsComponent.get_buffers()->num_buffers() == 2);
    }
}

TEST_CASE("GraphicsComponent Functionality", "[GraphicsComponent]"){
    EffectBuffer::GLErrorCheck();
    SECTION("Draw doesn't fail test"){
        EffectManager::GetInstance().Enable();

        GraphicsComponent graphicsComponent = GraphicsComponent();
        graphicsComponent.Draw(true);

        REQUIRE(!EffectBuffer::GLErrorCheck());
    }
}
