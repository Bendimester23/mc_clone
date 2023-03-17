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

    WorldRenderer::WorldRenderer() : m_AtlasTexture("atlas3"), m_ChunkShader("chunk_solid"), m_World()
    {
        this->m_ChunkShader.Reload();

        for (int x = 0; x < 15; x++) {
            for (int z = 0; z < 15; z++) {
                for (int y = 0; y < 4; y++) {
                    this->m_World.GenerateChunk(world::ChunkCoord { .x = x, .y = y, .z = z });
                }
            }
        }
    }

    void WorldRenderer::Update(double delta, glm::vec3 p)
    {
        this->m_World.Update(delta);

        p.x = std::round(p.x);
        p.y = std::round(p.y);
        p.z = std::round(p.z);

        this->m_World.m_CurrPos = world::ChunkCoord {static_cast<int>(p.x), static_cast<int>(p.y), static_cast<int>(p.z)};

        if (p != this->m_PrevPos) {
            this->m_World.m_NeedsRecheck = true;
        }
        this->m_PrevPos = p;
    }

    void WorldRenderer::Render(double delta, glm::mat4 matrix, bool wireframe)
    {
        this->m_ChunkShader.Bind();
        this->m_AtlasTexture.Bind();

        this->m_ChunkShader.SetUniformMat4("projectionMatrix", matrix);
        this->m_ChunkShader.SetUniformBool("wireframe", wireframe);

        for (auto &e : this->m_World.m_Chunks) {
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
