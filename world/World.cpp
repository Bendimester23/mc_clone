//
// Created by bendi on 2022. 07. 05..
//

#include "World.h"

namespace world {
    void World::LoadChunk(int px, int py, int pz) {
        auto chunk = new chunk::Chunk(ChunkCoordinate{px, py, pz});
        if (py < 0) {
            for (int x = 0; x < 16; ++x) {
                for (int y = 0; y < 16; ++y) {
                    for (int z = 0; z < 16; ++z) {
                        chunk->SetBlock(BlockCoordinate(x, y, z), block::BLOCK_STONE);
                    }
                }
            }
        } else {
            for (int x = 0; x < 16; ++x) {
                for (int z = 0; z < 16; ++z) {
                    int h = std::sin(glm::radians(x + (float)z + px*16 + pz*16)) * 30 + 8 - py*16;
                    if (h > 15) h = 15;
                    for (int y = 0; y < h; ++y) {
                        chunk->SetBlock(BlockCoordinate(x, y, z), block::BLOCK_GRASS);
                    }
                }
            }
        }
        auto mesh = chunk::ChunkMesh(chunk);
        this->meshes.push_back(mesh);
    }

    void World::Render() {
        this->atlasTexture.Bind();
        this->chunkShader.Bind();
        this->chunkShader.SetUniformMat4("projectionMat", this->cam->GetMatrix());
        for (auto &item: this->meshes) {
            auto pos = item.m_Chunk->m_Position;
            this->chunkShader.SetUniformVec3("chunkPos", glm::vec3(pos.x, pos.y, pos.z));

            item.Bind();

            item.Render();

            item.UnBind();
        }
        this->chunkShader.UnBind();
        this->atlasTexture.Unbind();
    }

    void World::Init() {
        this->chunkShader.Reload();
        this->atlasTexture.Load("atlas2.png");
        for (int x = -15; x < 15; ++x) {
            for (int y = -5; y < 5; ++y) {
                for (int z = -15; z < 15; ++z) {
                    this->LoadChunk(x, y, z);
                }
            }
        }
    }

    World::World(Camera *c) : chunkShader("chunk"), cam(c) {
    }

    void World::Update() {
        double start = glfwGetTime();
        for (auto &item: this->meshes) {
            if (item.Update() && glfwGetTime() - start > 0.05) break;
        }
    }

    bool World::IsAirAt(glm::vec3 pos) {
        auto chPos = ChunkCoordinate(pos.x / 16, pos.y / 16, pos.z / 16);
        for (auto &item: this->meshes) {
            if (item.m_Chunk->m_Position == chPos) {
                return item.IsAirAt(
                        BlockCoordinate(abs((int) pos.x % 16), abs((int) pos.y % 16), abs((int) pos.z % 16)));
            }
        }
        return false;
    }
}
