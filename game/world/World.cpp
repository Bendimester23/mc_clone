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
        while (this->m_UploadQueue.size() > 0)
        {
            auto to_upload = this->m_UploadQueue.front();
            to_upload->Upload();
            auto e = to_upload->GetPosition();
            spdlog::info("Uploading chunk X: {}, Y: {}, Z: {}", e.x, e.y, e.z);
            this->m_UploadQueue.pop_front();
        }
        this->m_UploadQueue.clear();
    }

    void World::GenerateChunk(ChunkCoord pos)
    {
        auto ch = Chunk(pos);
        for (char x = 0; x < 16; x++)
        {
            for (char z = 0; z < 16; z++)
            {
                ch.SetBlockAt(x, 0, z, 1);
            }
        }

        this->m_Chunks.push_back(ch);
        this->m_BuildQueue.push_back(&this->m_Chunks.back());
    }

    Chunk *World::GetChunk(ChunkCoord pos)
    {
        for (auto &e : this->m_Chunks)
        {
            if (e.GetPosition() == pos)
                return &e;
        }
        return nullptr;
    }

    void World::ChunkBuildWorker(bool *running)
    {
        while (*running)
        {
            while (this->m_BuildQueue.size() == 0 && *running)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            if (!(*running))
                break;
            auto to_build = this->m_BuildQueue.front();

            mesh::ChunkMeshBuilder mb;

            char base = 2;
            char esab = 3;

            mb.AddFace(
                mesh::VertexData{.x = 2, .y = 1, .z = 1, .u = base, .v = 1, .light = 15, .side = 3},
                mesh::VertexData{.x = 2, .y = 1, .z = 2, .u = esab, .v = 1, .light = 15, .side = 3},
                mesh::VertexData{.x = 2, .y = 2, .z = 2, .u = esab, .v = 0, .light = 15, .side = 3},
                mesh::VertexData{.x = 2, .y = 2, .z = 1, .u = base, .v = 0, .light = 15, .side = 3},
                false);

            for (char x1 = 0; x1 < 16; x1++)
            {
                for (char y1 = 0; y1 < 16; y1++)
                {
                    for (char z1 = 0; z1 < 16; z1++)
                    {
                        char x2 = x1 + 1;
                        char y2 = y1 + 1;
                        char z2 = z1 + 1;

                        if (to_build->GetBlockAt(x1, y1, z1) == 0) break;

                        if (to_build->GetBlockAt(x1 + 1, y1, z1) == 0)
                        {
                            mb.AddFace(
                                mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = base, .v = 1, .light = 15, .side = 3},
                                mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = esab, .v = 1, .light = 15, .side = 3},
                                mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = esab, .v = 0, .light = 15, .side = 3},
                                mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = base, .v = 0, .light = 15, .side = 3},
                                false);
                        }

                        if (to_build->GetBlockAt(x1 - 1, y1, z1) == 0)
                        {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = base, .v = 1, .light = 15, .side = 3},
                                mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = esab, .v = 1, .light = 15, .side = 3},
                                mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = esab, .v = 0, .light = 15, .side = 3},
                                mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = base, .v = 0, .light = 15, .side = 3},
                                true);
                        }

                        if (to_build->GetBlockAt(x1, y1, z1 + 1) == 0)
                        {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = base, .v = 1, .light = 15, .side = 4},
                                mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = base, .v = 0, .light = 15, .side = 4},
                                mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = esab, .v = 0, .light = 15, .side = 4},
                                mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = esab, .v = 1, .light = 15, .side = 4},
                                false);
                        }

                        if (to_build->GetBlockAt(x1, y1, z1 - 1) == 0)
                        {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = base, .v = 1, .light = 15, .side = 4},
                                mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = base, .v = 0, .light = 15, .side = 4},
                                mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = esab, .v = 0, .light = 15, .side = 4},
                                mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = esab, .v = 1, .light = 15, .side = 4},
                                true);
                        }

                        if (to_build->GetBlockAt(x1, y1 + 1, z1) == 0)
                        {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = 1, .v = 0, .light = 15, .side = 0},
                                mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = 1, .v = 1, .light = 15, .side = 0},
                                mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = 2, .v = 1, .light = 15, .side = 0},
                                mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = 2, .v = 0, .light = 15, .side = 0},
                                true);
                        }

                        if (to_build->GetBlockAt(x1, y1 - 1, z1) == 0)
                        {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = 1, .v = 0, .light = 15, .side = 0},
                                mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = 1, .v = 1, .light = 15, .side = 0},
                                mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = 2, .v = 1, .light = 15, .side = 0},
                                mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = 2, .v = 0, .light = 15, .side = 0},
                                false);
                        }
                    }
                }
            }

            to_build->m_Vertices = mb.GetVertices();
            to_build->m_Indices = mb.GetIndices();

            auto pos = to_build->GetPosition();
            spdlog::info("Got chunk to build (X: {}, Y: {}, Z: {})", pos.x, pos.y, pos.z);
            this->m_UploadQueue.push_back(to_build);
            this->m_BuildQueue.pop_front();
        }
    }

    std::deque<Chunk> World::GetChunks()
    {
        return this->m_Chunks;
    }
} // namespace world
