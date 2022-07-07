//
// Created by bendi on 2022. 07. 05..
//

#include "Texture.h"
#include "spdlog/spdlog.h"

void Texture::Load(const std::string& name) {
    int width, height, channels;
    unsigned char *img = stbi_load(("../assets/textures/" + name).c_str(), &width, &height, &channels, 0);
    if(img == nullptr) {
        printf("Error in loading the image\n");
        exit(1);
    }
    spdlog::info("Loaded image with a width of {}px, a height of {}px and {} channels", width, height, channels);
    this->m_Width = width;
    this->m_Height = height;

    glGenTextures(1, &this->m_Id);

    glBindTexture(GL_TEXTURE_2D, this->m_Id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    stbi_image_free(img);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() {
    if (this->m_Bound) return;
    this->m_Bound = true;

    glBindTexture(GL_TEXTURE_2D, this->m_Id);
}

void Texture::Unbind() {
    if (!this->m_Bound) return;
    this->m_Bound = false;

    glBindTexture(GL_TEXTURE_2D, 0);
}
