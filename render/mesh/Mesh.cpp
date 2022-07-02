//
// Created by bendi on 2022. 07. 02..
//

#include <cstring>
#include "Mesh.h"

Mesh::Mesh() {
    this->m_VertexFormat = VF_NONE;
}

void Mesh::From(MeshBuilder mb) {
    this->m_VertexFormat = VF_POSITION;

    auto vert = mb.GetVertices();
    float vertices[vert.size()];
    auto ind = mb.GetIndices();
    unsigned short indices[ind.size()];

    memcpy(vertices, vert.data(), vert.size() * sizeof(float));
    memcpy(indices, ind.data(), ind.size() * sizeof(unsigned short));

    this->Upload(vertices, indices, (int)vert.size(), (int)ind.size());
}

void Mesh::Upload(void *vertices, void *indices, int vertexCount, int indexCount) {
    glGenVertexArrays(1, &this->m_VAO);

    int size = this->m_VertexFormat.size;

    glGenBuffers(1, &this->m_VBO);
    glBindVertexArray(this->m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount*4, vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &this->m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)indexCount*sizeof(unsigned short), indices, GL_STATIC_DRAW);

    long offset = 0;

    for (int i = 0; i < 16; i++) {
        if (this->m_VertexFormat.attributes[i].size == 0) break;
        auto attr = this->m_VertexFormat.attributes[i];

        glVertexAttribPointer(attr.location, attr.count, attr.gl_type, GL_FALSE, size, (void *) &offset);
        glEnableVertexAttribArray(0);
        offset += attr.size;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::Mesh(VertexFormat vf, void* vertices, unsigned short* indices, int vertexCount, int indexCount) {
    this->m_VertexFormat = vf;
    this->Upload(vertices, indices, vertexCount, indexCount);
}

void Mesh::Bind() {
    if (this->m_Bound) return;
    this->m_Bound = true;
    glBindVertexArray(this->m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IndexBuffer);
}

void Mesh::UnBind() {
    if (!this->m_Bound) return;
    this->m_Bound = false;
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
