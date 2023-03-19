//
// Created by bendi on 2022.08.15..
//

#include "WorldRenderer.h"

namespace renderer {
    //+z north
    //-z south
    //+x east
    //-x west

    WorldRenderer::WorldRenderer() : m_AtlasTexture("atlas3"), m_ChunkShader("chunk_solid"), m_World() {
        this->m_ChunkShader.Reload();

//        for (int x = 0; x < 20; x++) {
//            for (int z = 0; z < 20; z++) {
//                for (int y = -2; y < 6; y++) {
//                    this->m_World.GenerateChunk(world::ChunkCoord{.x = x, .y = y, .z = z});
//                }
//            }
//        }
    }

    void WorldRenderer::Update(double delta, glm::vec3 p) {
        this->m_World.Update(delta);

        int pX = static_cast<int>(std::round(p.x)) >> 4;
        int pY = static_cast<int>(std::round(p.y)) >> 4;
        int pZ = static_cast<int>(std::round(p.z)) >> 4;


        using namespace std::chrono_literals;

        if (this->m_World.m_CurrPosMutex.try_lock_for(3ms)) {
            this->m_World.m_CurrPos = world::ChunkCoord{pX, pY, pZ};
            this->m_World.m_CurrPosMutex.unlock();
        }
//
//        if (this->m_World.m_CurrPos != this->m_PrevPos) {
//            this->m_World.m_NeedsRecheck = true;
////            auto chp = this->m_World.m_CurrPos;
////            spdlog::info("Chunk Pos changed: {} {} {}", chp.x, chp.y, chp.z);
//        }
//        this->m_PrevPos = this->m_World.m_CurrPos;
    }

    void WorldRenderer::Render(double delta, glm::mat4 matrix, bool wireframe) {
        this->m_ChunkShader.Bind();
        this->m_AtlasTexture.Bind();

        this->m_ChunkShader.SetUniformMat4("projectionMatrix", matrix);
        this->m_ChunkShader.SetUniformBool("wireframe", wireframe);

        this->m_World.m_ChunksMutex.lock();
        for (auto &e: this->m_World.m_Chunks) {
            auto pos = e.GetPosition();
            this->m_ChunkShader.SetUniformVec3("chunkPos", glm::vec3(pos.x, pos.y, pos.z));
            e.Render();
        }
        this->m_World.m_ChunksMutex.unlock();

        this->m_AtlasTexture.UnBind();
        this->m_ChunkShader.UnBind();
    }

    world::World *WorldRenderer::GetWorld() {
        return &this->m_World;
    }
} // renderer
