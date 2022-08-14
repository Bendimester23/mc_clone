//
// Created by bendi on 2022.08.13..
//

#ifndef MC_CLONE_2_SHADERPROGRAM_H
#define MC_CLONE_2_SHADERPROGRAM_H
#include <string>
#include <glad/glad.h>
#include <map>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include "../../config.h"
#include "../../utils/files.h"

namespace gl {

    class ShaderProgram {
        GLuint m_Id;
        std::string m_Name;
        bool m_Bound;
        bool m_HasError;
        std::map<std::string, GLint> m_UniformCache;

        GLint GetUniformLocation(const std::string& name);

        GLuint CreateShader(const std::string& path, GLuint type);

    public:
        explicit ShaderProgram(std::string  name);

        void Bind();

        void UnBind();

        bool Reload();

        void SetUniformMat4(const std::string& name, glm::mat4 value);
    };

} // gl

#endif //MC_CLONE_2_SHADERPROGRAM_H
