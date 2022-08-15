//
// Created by bendi on 2022.08.14..
//

#ifndef MC_CLONE_2_TEXTURE_H
#define MC_CLONE_2_TEXTURE_H

#include <string>
#include "../../config.h"
#include "glad/glad.h"
#include "spdlog/spdlog.h"
#include "stb_image.h"

namespace gl {

    class Texture {
        GLuint m_Id;
        bool m_Bound;
        std::string m_Name;

    public:
        Texture(const std::string& name);

        void Bind();

        void UnBind();
    };

} // gl

#endif //MC_CLONE_2_TEXTURE_H
