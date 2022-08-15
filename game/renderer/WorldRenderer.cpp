//
// Created by bendi on 2022.08.15..
//

#include "WorldRenderer.h"

namespace renderer {

    WorldRenderer::WorldRenderer() : m_AtlasTexture("atlas2"), m_ChunkShader("chunk_solid") {
        this->m_ChunkShader.Reload();

        mesh::ChunkMeshBuilder b;

        char base = 2;

        b.AddFace(
                mesh::VertexData{.x =  0, .y = 0, .z = 1, .u = base, .v = 0, .light = 15, .side = 0},
                mesh::VertexData{.x =  0, .y = 1, .z = 1, .u = base, .v = 1, .light = 15, .side = 0},
                mesh::VertexData{.x =  1, .y = 1, .z = 1, .u = static_cast<char>(base +
                                                                                 1), .v = 1, .light = 15, .side = 0},
                mesh::VertexData{.x =  1, .y = 0, .z = 1, .u = static_cast<char>(base +
                                                                                 1), .v = 0, .light = 15, .side = 0},
                false);

        b.AddFace(
                mesh::VertexData{.x =  0, .y = 0, .z = 0, .u = base, .v = 0, .light = 15, .side = 0},
                mesh::VertexData{.x =  0, .y = 1, .z = 0, .u = base, .v = 1, .light = 15, .side = 0},
                mesh::VertexData{.x =  1, .y = 1, .z = 0, .u = static_cast<char>(base +
                                                                                 1), .v = 1, .light = 15, .side = 0},
                mesh::VertexData{.x =  1, .y = 0, .z = 0, .u = static_cast<char>(base +
                                                                                 1), .v = 0, .light = 15, .side = 0},
                true);

        unsigned int vertices[b.GetVertices().size()];
        memcpy(vertices, b.GetVertices().data(), b.GetVertices().size() * sizeof(unsigned int));

        this->m_IndicesCount = b.GetIndices().size();

        unsigned short indices[m_IndicesCount];
        memcpy(indices, b.GetIndices().data(), m_IndicesCount * sizeof(unsigned short));

        glGenVertexArrays(1, &this->VAO);

        glGenBuffers(1, &this->VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, b.GetVertices().size() * sizeof(unsigned int), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &this->indexVBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, b.GetIndices().size() * sizeof(unsigned short), indices, GL_STATIC_DRAW);

        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(unsigned int), nullptr);
        glEnableVertexAttribArray(0);
    }

    void WorldRenderer::Update(double delta) {

    }

    void WorldRenderer::Render(double delta, glm::mat4 matrix) {
        this->m_ChunkShader.Bind();
        this->m_AtlasTexture.Bind();

        this->m_ChunkShader.SetUniformMat4("projectionMatrix", matrix);
        this->m_ChunkShader.SetUniformVec3("chunkPos", glm::vec3(0, 0, 0));

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
        //Rendering code goes here
        glDrawElements(
                GL_TRIANGLES,
                (int) m_IndicesCount,
                GL_UNSIGNED_SHORT,
                nullptr
        );
        glBindVertexArray(0);

        this->m_AtlasTexture.UnBind();
        this->m_ChunkShader.UnBind();
    }
} // renderer