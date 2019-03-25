//
// Created by yanik on 24/03/19.
//

#ifndef PROJECT_TEXTURED_OBJECT_H
#define PROJECT_TEXTURED_OBJECT_H

#include <string>
#include "texture.h"
#include "resource_user.h"

class TexturedObject : public ResourceUser
{
public:
    TexturedObject();

    std::string get_texture();
    virtual void set_texture(std::string newTex);

    bool LoadExternalResources();
    bool UnloadExternalResources();
protected:
    bool m_texture_loaded;
    std::string m_texture;
    Texture* m_tex_ptr;
};

#endif //PROJECT_TEXTURED_OBJECT_H
