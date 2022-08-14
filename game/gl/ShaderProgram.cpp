//
// Created by bendi on 2022.08.13..
//

#include "ShaderProgram.h"

#include <utility>

namespace gl {
    void ShaderProgram::Bind() {
        if (this->m_HasError) {
            spdlog::warn("Tried to bind shader \"{}\" with compilation errors!", this->m_Name);
            return;
        }
        if (this->m_Bound) {
            spdlog::warn("Tried to bind shader \"{}\" twice, you may have just forgot to unbind it before!", this->m_Name);
            return;
        }
        glUseProgram(m_Id);
        this->m_Bound = true;
    }

    void ShaderProgram::UnBind() {
        if (!this->m_Bound) {
            spdlog::warn("Tried to unbind shader \"{}\" which wasn't bound!", this->m_Name);
            return;
        }
        glUseProgram(0);
        this->m_Bound = false;
    }

    ShaderProgram::ShaderProgram(std::string name)
    : m_Id(0), m_Name(std::move(name)), m_Bound(false), m_HasError(false)
    {
    }

    bool ShaderProgram::Reload() {
        auto vertex = CreateShader(SHADERS_LOC+m_Name+"/vert.glsl", GL_VERTEX_SHADER);
        auto fragment = CreateShader(SHADERS_LOC+m_Name+"/frag.glsl", GL_FRAGMENT_SHADER);

        this->m_Id = glCreateProgram();
        glAttachShader(this->m_Id, vertex);
        glAttachShader(this->m_Id, fragment);

        glLinkProgram(this->m_Id);

        int success;
        glGetProgramiv(this->m_Id, GL_LINK_STATUS, &success);

        if (!success) {
            char log[512];
            glGetProgramInfoLog(this->m_Id, 512, nullptr, log);
            spdlog::error("Error linking program: {}", log);
            this->m_HasError = true;
            return true;
        }
        spdlog::info("Loaded shader \"{}\"", m_Name);
        return false;
    }

    GLuint ShaderProgram::CreateShader(const std::string &path, GLuint type) {
        GLuint shader = glCreateShader(type);
        auto src = utils::ReadFile(path);
        auto cSrc = src.c_str();
        glShaderSource(shader, 1, &cSrc, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char log[512];
            glGetShaderInfoLog(shader, 512, nullptr, log);
            spdlog::error("Error compiling shader: {}", log);
            this->m_HasError = true;
        }

        return shader;
    }

    GLint ShaderProgram::GetUniformLocation(const std::string& name) {
        if (this->m_UniformCache.find(name) == this->m_UniformCache.end()) this->m_UniformCache[name] = glGetUniformLocation(this->m_Id, name.c_str());
        return this->m_UniformCache[name];
    }

    void ShaderProgram::SetUniformMat4(const std::string &name, glm::mat4 value) {
        if (!this->m_Bound) {
            spdlog::warn(R"(Tried to set uniform "{}" on unbound shader "{}"!)", name, this->m_Name);
            return;
        }
        glUniformMatrix4fv(this->GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
    }
} // gl