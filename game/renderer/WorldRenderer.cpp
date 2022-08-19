//
// Created by bendi on 2022.08.15..
//

#include "WorldRenderer.h"

namespace renderer
{

    //+z north
    //-z south
    //+x east
    //-x west

    WorldRenderer::WorldRenderer() : m_AtlasTexture("atlas2"), m_ChunkShader("chunk_solid"), b(), m_World()
    {
        this->m_ChunkShader.Reload();

        for (size_t x = 0; x < 16; x++)
        {
            for (size_t z = 0; z < 16; z++)
            {
                AddBlock(x, 0, z);
            }
        }

        world::Chunk ch;
        ch.SetBlockAt(4, 2, 0, 69);

        assert(ch.GetBlockAt(4, 2, 0) == 69);

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

        this->m_World.GenerateChunk(world::ChunkCoord { .x = 0, .y = 0, .z = 0 });
    }

    void WorldRenderer::Update(double delta)
    {
        this->m_World.Update(delta);
    }

    void WorldRenderer::Render(double delta, glm::mat4 matrix, bool wireframe)
    {
        this->m_ChunkShader.Bind();
        this->m_AtlasTexture.Bind();

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);

        this->m_ChunkShader.SetUniformMat4("projectionMatrix", matrix);
        this->m_ChunkShader.SetUniformBool("wireframe", wireframe);

        for (size_t i = 0; i < 10; i++)
        {
            for (size_t k = 0; k < 10; k++)
            {
                this->m_ChunkShader.SetUniformVec3("chunkPos", glm::vec3(i, 0, k));

                glDrawElements(
                    GL_TRIANGLES,
                    (int)m_IndicesCount,
                    GL_UNSIGNED_SHORT,
                    nullptr);
            }
        }

        glBindVertexArray(0);

        this->m_AtlasTexture.UnBind();
        this->m_ChunkShader.UnBind();
    }

    void WorldRenderer::AddBlock(char x1, char y1, char z1)
    {

        char x2 = x1 + 1;
        char y2 = y1 + 1;
        char z2 = z1 + 1;

        char base = 2;
        char esab = 3;

        if (z1 == 15)
        {
            // North
            b.AddFace(
                mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = base, .v = 1, .light = 15, .side = 4},
                mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = base, .v = 0, .light = 15, .side = 4},
                mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = esab, .v = 0, .light = 15, .side = 4},
                mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = esab, .v = 1, .light = 15, .side = 4},
                false);
        }

        if (z1 == 0)
        {
            // South
            b.AddFace(
                mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = base, .v = 1, .light = 15, .side = 1},
                mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = base, .v = 0, .light = 15, .side = 1},
                mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = esab, .v = 0, .light = 15, .side = 1},
                mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = esab, .v = 1, .light = 15, .side = 1},
                true);
        }
        // Top
        b.AddFace(
            mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = 1, .v = 0, .light = 15, .side = 0},
            mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = 1, .v = 1, .light = 15, .side = 0},
            mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = 2, .v = 1, .light = 15, .side = 0},
            mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = 2, .v = 0, .light = 15, .side = 0},
            true);

        // Bottom
        b.AddFace(
            mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = 4, .v = 0, .light = 15, .side = 5},
            mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = 4, .v = 1, .light = 15, .side = 5},
            mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = 5, .v = 1, .light = 15, .side = 5},
            mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = 5, .v = 0, .light = 15, .side = 5},
            false);

        if (x1 == 15)
        {
            // East
            b.AddFace(
                mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = base, .v = 1, .light = 15, .side = 3},
                mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = esab, .v = 1, .light = 15, .side = 3},
                mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = esab, .v = 0, .light = 15, .side = 3},
                mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = base, .v = 0, .light = 15, .side = 3},
                false);
        }

        if (x1 == 0)
        {
            // West
            b.AddFace(
                mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = base, .v = 1, .light = 15, .side = 2},
                mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = esab, .v = 1, .light = 15, .side = 2},
                mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = esab, .v = 0, .light = 15, .side = 2},
                mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = base, .v = 0, .light = 15, .side = 2},
                true);
        }
    }

    world::World* WorldRenderer::GetWorld() {
        return &this->m_World;
    }
} // renderer