//
// Created by bendi on 2022. 07. 05..
//

#include "ChunkMesh.h"
#include "../../Game.h"

namespace chunk {
    ChunkMesh::ChunkMesh(Chunk *chunk) {
        this->m_Chunk = chunk;
    }

    void ChunkMesh::Bind() {
        if (this->m_Bound) return;
        this->m_Bound = true;
        glBindVertexArray(this->m_VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IndexBuffer);
    }

    void ChunkMesh::UnBind() {
        if (!this->m_Bound) return;
        this->m_Bound = false;
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    bool ChunkMesh::Update() {
        if (!this->m_Chunk->IsDirty()) return false;
        auto start = glfwGetTime();
        this->Rebuild();
        //spdlog::info("Chunk mesh rebuild took: {}ms", (glfwGetTime() - start) * 1000);
        return true;
    }


    void ChunkMesh::Rebuild() {
        ChunkMeshBuilder mb;
        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                for (int z = 0; z < 16; ++z) {
                    auto bl = this->m_Chunk->GetBlock(BlockCoordinate(x, y, z));
                    if (!bl.IsOpaque()) continue;
                    if (IsAirAt(BlockCoordinate(x, y + 1, z))) {
                        auto uv = bl.m_TopTexture;
                        mb.AddQuad(
                                ChunkVertex(x, y + 1, z, uv.u, uv.v, 15),
                                ChunkVertex(x + 1, y + 1, z, uv.u + 1, uv.v, 15),
                                ChunkVertex(x + 1, y + 1, z + 1, uv.u + 1, uv.v + 1, 15),
                                ChunkVertex(x, y + 1, z + 1, uv.u, uv.v + 1, 15),
                                false
                        );
                    }
                    if (IsAirAt(BlockCoordinate(x, y - 1, z))) {
                        auto uv = bl.m_BottomTexture;
                        mb.AddQuad(
                                ChunkVertex(x, y, z, uv.u, uv.v, 13),
                                ChunkVertex(x + 1, y, z, uv.u + 1, uv.v, 13),
                                ChunkVertex(x + 1, y, z + 1, uv.u + 1, uv.v + 1, 13),
                                ChunkVertex(x, y, z + 1, uv.u, uv.v + 1, 13),
                                true
                        );
                    }
                    if (IsAirAt(BlockCoordinate(x + 1, y, z))) {
                        auto uv = bl.m_SideTexture;
                        mb.AddQuad(
                                ChunkVertex(x + 1, y, z, uv.u + 1, uv.v + 1, 14),
                                ChunkVertex(x + 1, y + 1, z, uv.u + 1, uv.v, 14),
                                ChunkVertex(x + 1, y + 1, z + 1, uv.u, uv.v, 14),
                                ChunkVertex(x + 1, y, z + 1, uv.u, uv.v + 1, 14),
                                true
                        );
                    }
                    if (IsAirAt(BlockCoordinate(x - 1, y, z))) {
                        auto uv = bl.m_SideTexture;
                        mb.AddQuad(
                                ChunkVertex(x, y, z, uv.u + 1, uv.v + 1, 14),
                                ChunkVertex(x, y + 1, z, uv.u + 1, uv.v, 14),
                                ChunkVertex(x, y + 1, z + 1, uv.u, uv.v, 14),
                                ChunkVertex(x, y, z + 1, uv.u, uv.v + 1, 14),
                                false
                        );
                    }
                    if (IsAirAt(BlockCoordinate(x, y, z + 1))) {
                        auto uv = bl.m_SideTexture;
                        mb.AddQuad(
                                ChunkVertex(x, y, z + 1, uv.u + 1, uv.v + 1, 14),
                                ChunkVertex(x, y + 1, z + 1, uv.u + 1, uv.v, 14),
                                ChunkVertex(x + 1, y + 1, z + 1, uv.u, uv.v, 14),
                                ChunkVertex(x + 1, y, z + 1, uv.u, uv.v + 1, 14),
                                false
                        );
                    }
                    if (IsAirAt(BlockCoordinate(x, y, z - 1))) {
                        auto uv = bl.m_SideTexture;
                        mb.AddQuad(
                                ChunkVertex(x, y, z, uv.u + 1, uv.v + 1, 14),
                                ChunkVertex(x, y + 1, z, uv.u + 1, uv.v, 14),
                                ChunkVertex(x + 1, y + 1, z, uv.u, uv.v, 14),
                                ChunkVertex(x + 1, y, z, uv.u, uv.v + 1, 14),
                                true
                        );
                    }
                }
            }
        }
        this->m_Chunk->SetDirty(false);
        mb.Trim();

        this->indicesCount = mb.GetIndicesCount();

        auto vert = mb.GetVertices();
        unsigned int vertices[vert.size()];
        auto ind = mb.GetIndices();
        unsigned short indices[ind.size()];

//    for (auto &item : mb.GetVertices()) {
//        spdlog::info("Vertex: {}", item);
//    }

        memcpy(vertices, vert.data(), vert.size() * sizeof(unsigned int));
        memcpy(indices, ind.data(), ind.size() * sizeof(unsigned short));

        glGenVertexArrays(1, &this->m_VAO);

        glGenBuffers(1, &this->m_VBO);
        glBindVertexArray(this->m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
        glBufferData(GL_ARRAY_BUFFER, (long) vert.size() * (long) sizeof(unsigned int), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &this->m_IndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long) ind.size() * 2, indices, GL_STATIC_DRAW);

        //Do you see that 'I' in the method name?
        //That took half an hour to figure out
        //Without that, it breaks
        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(unsigned int), nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void ChunkMesh::Render() {
        glDrawElements(
                GL_TRIANGLES,
                this->indicesCount,
                GL_UNSIGNED_SHORT,
                (void *) nullptr
        );
    }

    bool ChunkMesh::IsAirAt(BlockCoordinate pos) {
        //FIXME should return true at the end of the world
        if (pos.x < 0 || pos.y < 0 || pos.z < 0) return false;//Game::GetInstance()->world.IsAirAt(pos+this->m_Chunk->m_Position);
        if (pos.x > 15 || pos.y > 15 || pos.z > 15) return false;//Game::GetInstance()->world.IsAirAt(pos+this->m_Chunk->m_Position);
        return !this->m_Chunk->GetBlock(pos).IsOpaque();
    }
}
