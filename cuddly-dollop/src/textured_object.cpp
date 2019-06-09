//
// Created by yanik on 24/03/19.
//

#include "textured_object.h"

#include <iostream>

#include "resource_manager.h"

TexturedObject::TexturedObject() : m_texture_loaded(false), m_tex_ptr(nullptr), m_texture("") {

}

std::string TexturedObject::get_texture() {
    return m_texture;
}

void TexturedObject::set_texture(std::string newTex) {
    m_texture = newTex;
    LoadExternalResources();
    m_tex_ptr = ResourceManager::GetInstance().GetTexture(m_texture);
}

bool TexturedObject::LoadExternalResources() {
    if (!ResourceManager::GetInstance().LoadTexture(m_texture))
    {
        // If its not something stupid, send error message
        if (!(m_texture == "" || m_texture[m_texture.size() - 1] == '\\' || m_texture[m_texture.size() - 1] == '/'))
            std::cout << "Texture '" << m_texture << "' could not load" << std::endl;
        m_texture_loaded = false;
        return false;
    }

    m_texture_loaded = true;
    return true;
}

bool TexturedObject::UnloadExternalResources() {
    m_texture_loaded = false;
    std::cout << "UnloadExternalResources (GraphicsComponent) isn't implemented!" << std::endl;
    return true;
}
