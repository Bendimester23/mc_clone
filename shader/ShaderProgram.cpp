//
// Created by bendi on 2022. 06. 27..
//

#include "ShaderProgram.h"

#include <utility>

void ShaderProgram::Bind() {
    if (this->m_HasError) {
        spdlog::warn("Tried to bind shader \"{}\" with compilation errors!", this->m_Name);
        return;
    }
    if (this->m_Bound) {
        spdlog::warn("Tried to bind shader \"{}\" twice, you may have just forgot to unbind it before!", this->m_Name);
        return;
    }
    glUseProgram(id);
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

ShaderProgram::ShaderProgram(std::string name) {
    this->m_Name = std::move(name);
    this->m_Bound = false;
    this->m_HasError = false;
}

void ShaderProgram::Reload() {
    auto vertex = CreateShader(SHADERS_LOC+m_Name+"/vert.glsl", GL_VERTEX_SHADER);
    auto fragment = CreateShader(SHADERS_LOC+m_Name+"/frag.glsl", GL_FRAGMENT_SHADER);

    this->id = glCreateProgram();
    glAttachShader(this->id, vertex);
    glAttachShader(this->id, fragment);

    glLinkProgram(this->id);

    int success;
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);

    if (!success) {
        char log[512];
        glGetProgramInfoLog(this->id, 512, nullptr, log);
        spdlog::error("Error linking program: {}", log);
        this->m_HasError = true;
    }
}

GLuint ShaderProgram::CreateShader(const std::string &path, GLuint type) {
    GLuint shader = glCreateShader(type);
    auto src = utils::ReadFileToString(path.c_str());
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

GLuint ShaderProgram::GetUniformLocation(const std::string& name) {
    if (this->m_UniformCache.find(name) == this->m_UniformCache.end()) this->m_UniformCache[name] = glGetUniformLocation(this->id, name.c_str());
    return this->m_UniformCache[name];
}

void ShaderProgram::SetUniformMat4(std::string name, glm::mat4 value) {
    if (!this->m_Bound) {
        spdlog::warn(R"(Tried to set uniform "{}" on unbound shader "{}"!)", name, this->m_Name);
        return;
    }
    glUniformMatrix4fv(this->GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}
