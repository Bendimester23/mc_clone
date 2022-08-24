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

    WorldRenderer::WorldRenderer() : m_AtlasTexture("atlas2"), m_ChunkShader("chunk_solid"), m_World()
    {
        this->m_ChunkShader.Reload();

        for (int x = 0; x < 10; x++) {
            for (int z = 0; z < 10; z++) {
                for (int y = 0; y < 4; y++) {
                    this->m_World.GenerateChunk(world::ChunkCoord { .x = x, .y = y, .z = z });
                }
            }
        }
    }

    void WorldRenderer::Update(double delta, glm::vec3 p)
    {
        this->m_World.Update(delta);
        this->m_Frame+=1;

        p.x = (int)p.x;
        p.y = (int)p.y;
        p.z = (int)p.z;

        if (this->m_Frame >= 20 && this->m_World.GetChunk(world::ChunkCoord {(int)p.x, 0, (int)p.z}) == nullptr) {
            for (int y = -5; y < 5; y++) {
                this->m_World.GenerateChunk(world::ChunkCoord {(int)p.x, y, (int)p.z});
            }
            this->m_Frame = 0;
        }
        if (p != this->m_PrevPos) {
            auto pos = world::ChunkCoord {(int)p.x, (int)p.y, (int)p.z};
            auto chs = this->m_World.GetChunks();
            for (int i = 0; i < chs.size(); i++) {
                int dst = pos.maxDst(chs[i].GetPosition());
                if (dst > 8) {
                    auto ch = chs[i].GetPosition();
                    spdlog::info("Erasing chunk X: {}, Y: {}, Z: {} with dst: {}", ch.x, ch.y, ch.z, dst);
                    this->m_World.HideChunk(ch);
                }
            }
        }
        this->m_PrevPos = p;
    }

    void WorldRenderer::Render(double delta, glm::mat4 matrix, bool wireframe)
    {
        this->m_ChunkShader.Bind();
        this->m_AtlasTexture.Bind();

        this->m_ChunkShader.SetUniformMat4("projectionMatrix", matrix);
        this->m_ChunkShader.SetUniformBool("wireframe", wireframe);

        for (auto &e : this->m_World.GetChunks()) {
            auto pos = e.GetPosition();
            this->m_ChunkShader.SetUniformVec3("chunkPos", glm::vec3(pos.x, pos.y, pos.z));
            e.Render();
        }

        this->m_AtlasTexture.UnBind();
        this->m_ChunkShader.UnBind();
    }

    world::World* WorldRenderer::GetWorld() {
        return &this->m_World;
    }
} // renderer
