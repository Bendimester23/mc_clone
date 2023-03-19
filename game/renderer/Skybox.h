#ifndef MC_CLONE_2_SKYBOX_H
#define MC_CLONE_2_SKYBOX_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../gl/Texture.h"
#include "../gl/ShaderProgram.h"

namespace renderer {
    class Skybox {
        gl::Texture m_SkyboxTexture;
        gl::ShaderProgram m_SkyboxShader;
        GLuint VAO;
        GLuint VBO;
        GLuint indexVBO;
        size_t m_IndexCount;

    public:
        Skybox(float size);

        void Render(glm::mat4 projMat);
    };
} // namespace renderer


#endif