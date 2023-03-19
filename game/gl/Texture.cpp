//
// Created by bendi on 2022.08.14..
//

#include "Texture.h"

namespace gl {
    Texture::Texture(const std::string &name) : m_Id(0), m_Bound(false), m_Name(name) {
        int width, height, channels;
        unsigned char *img = stbi_load(fmt::format("{}{}.png", TEXTURE_LOC, name).c_str(), &width, &height, &channels,
                                       0);
        if (img == nullptr) {
            spdlog::error("Error loading texture \"{}\"!", name);
            exit(1);
        }

        glGenTextures(1, &this->m_Id);

        glBindTexture(GL_TEXTURE_2D, m_Id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

        stbi_image_free(img);

        spdlog::info("Loaded texture \"{}\"", name);
    }

    void Texture::Bind() {
        if (m_Bound) {
            spdlog::warn("Texture \"{}\" is already bound!", m_Name);
            return;
        }
        this->m_Bound = true;
        glBindTexture(GL_TEXTURE_2D, m_Id);
    }

    void Texture::UnBind() {
        if (!m_Bound) {
            spdlog::warn("Texture \"{}\" isn't bound!", m_Name);
            return;
        }
        this->m_Bound = false;
        glBindTexture(GL_TEXTURE_2D, 0);
    }
} // gl
