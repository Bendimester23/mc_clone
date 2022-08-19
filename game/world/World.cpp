#include "./World.h"

namespace world
{
    void World::Clear()
    {
        this->m_Chunks.clear();
        this->m_BuildQueue.clear();
        this->m_UploadQueue.clear();
    }

    void World::Update(double delta)
    {
        auto to_upload = this->m_UploadQueue.try_fetch_all();
        for (auto &e : to_upload)
        {
            spdlog::info("Uploading chunk X: {}, Y: {}, Z: {}", e.x, e.y, e.z);
        }
        this->m_UploadQueue.clear();
    }

    void World::GenerateChunk(ChunkCoord pos)
    {
        auto ch = Chunk();
        this->m_Chunks[pos] = ch;
        this->m_BuildQueue.push_back(pos);
    }

    void World::ChunkBuildWorker(bool* running)
    {
        while (*running)
        {
            auto to_build_pos = this->m_BuildQueue.fetch(running);

            this->m_UploadQueue.push_back(to_build_pos);
            spdlog::info("Got chunk to build (X: {}, Y: {}, Z: {})", to_build_pos.x, to_build_pos.y, to_build_pos.z);
        }
    }
} // namespace world
