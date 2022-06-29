//
// Created by bendi on 2022. 06. 27..
//
#pragma once
#ifndef MC_CLONE_SHADERPROGRAM_H
#define MC_CLONE_SHADERPROGRAM_H

#include <GL/glew.h>
#include <string>
#include <spdlog/spdlog.h>
#include <glm/mat4x4.hpp>
#include <map>
#include "../utils/files.h"
#include "glm/ext/matrix_float4x4.hpp"

static const std::string SHADERS_LOC = "../assets/shaders/";

class ShaderProgram {
    GLuint id;
    std::string m_Name;
    bool m_Bound;
    bool m_HasError;
    std::map<std::string, GLuint> m_UniformCache;

public:
    explicit ShaderProgram(std::string name);

    void Bind();

    void UnBind();

    void Reload();

    void SetUniformMat4(std::string name, glm::mat4 value);

private:
    GLuint GetUniformLocation(const std::string& name);

    GLuint CreateShader(const std::string& path, GLuint type);
};


#endif //MC_CLONE_SHADERPROGRAM_H
