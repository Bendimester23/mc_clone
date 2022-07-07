//
// Created by bendi on 2022. 07. 05..
//

#ifndef MC_CLONE_TEXTURE_H
#define MC_CLONE_TEXTURE_H


#include <string>
#include "GL/glew.h"
#include <stb_image.h>

struct Texture {
    GLuint m_Id;
    int m_Width, m_Height = 0;
    bool m_Bound = false;

    Texture() = default;

    void Load(const std::string& name);

    void Bind();

    void Unbind();
};


#endif //MC_CLONE_TEXTURE_H
