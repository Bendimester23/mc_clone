//
// Created by bendi on 2022. 07. 02..
//
#pragma once
#ifndef MC_CLONE_MESH_H
#define MC_CLONE_MESH_H

#include <GL/glew.h>
#include <string>
#include "MeshBuilder.h"

struct VertexAttribute {
    unsigned char location;
    int gl_type;
    int count;
    int size;
};

struct VertexFormat {
    int size;
    VertexAttribute *attributes;
};

static const VertexFormat VF_NONE = {0, {}};

static const VertexAttribute VA_POS_3D = VertexAttribute{
        .location = 0,
        .gl_type = GL_FLOAT,
        .count = 3,
        .size = 3 * sizeof(float)
};

static VertexAttribute VF_POSITION_ARR_STUFF[] ={VA_POS_3D};

static const VertexFormat VF_POSITION = {
        3 * sizeof(float),
        VF_POSITION_ARR_STUFF
};

class Mesh {
private:
    GLuint m_VAO{}, m_VBO{}, m_IndexBuffer{};
    bool m_Bound = false;
    VertexFormat m_VertexFormat;

public:
    Mesh();

    void From(MeshBuilder mb);

    Mesh(VertexFormat vf, void* vertices, unsigned short* indices, int vertexCount, int indexCount);

    void Bind();

    void UnBind();

    void Upload(void* vertices, void* indices, int vertexCount, int indexCount);
};


#endif //MC_CLONE_MESH_H
