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
                auto height = glm::simplex(glm::vec2((pos.x*16+x)*0.03f, (pos.z*16 + z)*0.03f)) * 10 + 20;
                height -= pos.y * 16;
                for (char y = 0; y < glm::min(height, 16.0f); y++)
                {
                    if (y == glm::ceil(height) - 1) ch.SetBlockAt(x, y, z, 3);
                    else if (glm::floor(height) - y < 4) ch.SetBlockAt(x, y, z, 2);
                    else ch.SetBlockAt(x, y, z, 1);
                }
            }
        }

        this->m_Chunks.push_back(ch);
        this->m_BuildQueue.push_back(&this->m_Chunks.back());
        this->m_BuildQueue.push_back(this->GetChunk(pos + ChunkCoord {1, 0, 0}));
        this->m_BuildQueue.push_back(this->GetChunk(pos + ChunkCoord {-1, 0, 0}));
        this->m_BuildQueue.push_back(this->GetChunk(pos + ChunkCoord {0, 1, 0}));
        this->m_BuildQueue.push_back(this->GetChunk(pos + ChunkCoord {0, -1, 0}));
        this->m_BuildQueue.push_back(this->GetChunk(pos + ChunkCoord {0, 0, 1}));
        this->m_BuildQueue.push_back(this->GetChunk(pos + ChunkCoord {0, 0, -1}));
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

    void World::ChunkGenerateWorker(bool *running)
    {
        while (*running)
        {
            while (this->m_GenerateQueue.size() == 0 && *running)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            if (!(*running)) break;

            auto pos = this->m_GenerateQueue.front();

            auto ch = Chunk(pos);
            for (char x = 0; x < 16; x++)
            {
                for (char z = 0; z < 16; z++)
                {
                    auto height = glm::simplex(glm::vec2((pos.x*16+x)*0.03f, (pos.z*16 + z)*0.03f)) * 10 + 20;
                    height -= pos.y * 16;
                    for (char y = 0; y < glm::min(height, 16.0f); y++)
                    {
                        if (y == glm::ceil(height) - 1) ch.SetBlockAt(x, y, z, 3);
                        else if (glm::floor(height) - y < 4) ch.SetBlockAt(x, y, z, 2);
                        else ch.SetBlockAt(x, y, z, 1);
                    }
                }
            }

            this->m_Chunks.push_back(ch);
            this->m_BuildQueue.push_back(&this->m_Chunks.back());
            this->m_BuildQueue.push_back(this->GetChunk(pos + ChunkCoord {1, 0, 0}));
            this->m_BuildQueue.push_back(this->GetChunk(pos + ChunkCoord {-1, 0, 0}));
            this->m_BuildQueue.push_back(this->GetChunk(pos + ChunkCoord {0, 1, 0}));
            this->m_BuildQueue.push_back(this->GetChunk(pos + ChunkCoord {0, -1, 0}));
            this->m_BuildQueue.push_back(this->GetChunk(pos + ChunkCoord {0, 0, 1}));
            this->m_BuildQueue.push_back(this->GetChunk(pos + ChunkCoord {0, 0, -1}));
            this->m_GenerateQueue.pop_front();
        }
    }

    void World::RebuildAll() {
        for (auto &e : this->m_Chunks) {
            this->m_BuildQueue.push_back(&e);
        }
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

            if (to_build == nullptr) {
                this->m_BuildQueue.pop_front();
                continue;
            }

            mesh::ChunkMeshBuilder mb;

            auto chy1 = this->GetChunk(to_build->GetPosition()+ChunkCoord{0, -1, 0});
            auto chy2 = this->GetChunk(to_build->GetPosition()+ChunkCoord{0,  1, 0});

            auto chx1 = this->GetChunk(to_build->GetPosition()+ChunkCoord{-1, 0, 0});
            auto chx2 = this->GetChunk(to_build->GetPosition()+ChunkCoord{ 1, 0, 0});

            auto chz1 = this->GetChunk(to_build->GetPosition()+ChunkCoord{0, 0, -1});
            auto chz2 = this->GetChunk(to_build->GetPosition()+ChunkCoord{0, 0,  1});

            for (char x1 = 0; x1 < 16; x1++)
            {
                for (char y1 = 0; y1 < 16; y1++)
                {
                    for (char z1 = 0; z1 < 16; z1++)
                    {
                        char x2 = x1 + 1;
                        char y2 = y1 + 1;
                        char z2 = z1 + 1;

                        auto curr_b = to_build->GetBlockAt(x1, y1, z1);

                        if (!curr_b.m_Opaque) continue;


                        char side_u1 = curr_b.m_Side.u;
                        char side_u2 = curr_b.m_Side.u+1;
                        char side_v1 = curr_b.m_Side.v;
                        char side_v2 = curr_b.m_Side.v+1;

                        char bott_u1 = curr_b.m_Bottom.u;
                        char bott_u2 = curr_b.m_Bottom.u+1;
                        char bott_v1 = curr_b.m_Bottom.v;
                        char bott_v2 = curr_b.m_Bottom.v+1;

                        char top_u1 = curr_b.m_Top.u;
                        char top_u2 = curr_b.m_Top.u+1;
                        char top_v1 = curr_b.m_Top.v;
                        char top_v2 = curr_b.m_Top.v+1;

                        //X+
                        if (x2 < 16 && !to_build->GetBlockAt(x1 + 1, y1, z1).m_Opaque)
                        {
                            mb.AddFace(
                                mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = side_u1, .v = side_v2, .light = 15, .side = 3},
                                mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = side_u2, .v = side_v2, .light = 15, .side = 3},
                                mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = side_u2, .v = side_v1, .light = 15, .side = 3},
                                mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = side_u1, .v = side_v1, .light = 15, .side = 3},
                                false);
                        } else if (x2 >= 16 && (chx2 == nullptr || !chx2->GetBlockAt(x2-16, y1, z1).m_Opaque)) {
                            mb.AddFace(
                                mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = side_u1, .v = side_v2, .light = 15, .side = 3},
                                mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = side_u2, .v = side_v2, .light = 15, .side = 3},
                                mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = side_u2, .v = side_v1, .light = 15, .side = 3},
                                mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = side_u1, .v = side_v1, .light = 15, .side = 3},
                                false);
                        }

                        //X-
                        if (x1 > 0 && !to_build->GetBlockAt(x1 - 1, y1, z1).m_Opaque)
                        {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = side_u1, .v = side_v2, .light = 15, .side = 3},
                                mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = side_u2, .v = side_v2, .light = 15, .side = 3},
                                mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = side_u2, .v = side_v1, .light = 15, .side = 3},
                                mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = side_u1, .v = side_v1, .light = 15, .side = 3},
                                true);
                        } else if (x1 <= 0 && (chx1 == nullptr || !chx1->GetBlockAt(x1+15, y1, z1).m_Opaque)) {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = side_u1, .v = side_v2, .light = 15, .side = 3},
                                mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = side_u2, .v = side_v2, .light = 15, .side = 3},
                                mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = side_u2, .v = side_v1, .light = 15, .side = 3},
                                mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = side_u1, .v = side_v1, .light = 15, .side = 3},
                                true);
                        }

                        //Z+
                        if (z2 < 16 && !to_build->GetBlockAt(x1, y1, z1 + 1).m_Opaque)
                        {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = side_u1, .v = side_v2, .light = 15, .side = 4},
                                mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = side_u1, .v = side_v1, .light = 15, .side = 4},
                                mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = side_u2, .v = side_v1, .light = 15, .side = 4},
                                mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = side_u2, .v = side_v2, .light = 15, .side = 4},
                                false);
                        } else if (z2 >= 16 && (chz2 == nullptr || !chz2->GetBlockAt(x1, y1, z2-16).m_Opaque)) {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = side_u1, .v = side_v2, .light = 15, .side = 4},
                                mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = side_u1, .v = side_v1, .light = 15, .side = 4},
                                mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = side_u2, .v = side_v1, .light = 15, .side = 4},
                                mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = side_u2, .v = side_v2, .light = 15, .side = 4},
                                false);
                        }

                        //Z-
                        if (z1 > 0 && !to_build->GetBlockAt(x1, y1, z1 - 1).m_Opaque)
                        {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = side_u1, .v = side_v2, .light = 15, .side = 4},
                                mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = side_u1, .v = side_v1, .light = 15, .side = 4},
                                mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = side_u2, .v = side_v1, .light = 15, .side = 4},
                                mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = side_u2, .v = side_v2, .light = 15, .side = 4},
                                true);
                        } else if (z1 <= 0 && (chz1 == nullptr || !chz1->GetBlockAt(x1, y1, z1+15).m_Opaque)) {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = side_u1, .v = side_v2, .light = 15, .side = 4},
                                mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = side_u1, .v = side_v1, .light = 15, .side = 4},
                                mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = side_u2, .v = side_v1, .light = 15, .side = 4},
                                mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = side_u2, .v = side_v2, .light = 15, .side = 4},
                                true);
                        }

                        //Top
                        if (y2 < 16 && !to_build->GetBlockAt(x1, y1 + 1, z1).m_Opaque)
                        {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = top_u1, .v = top_v1, .light = 15, .side = 0},
                                mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = top_u1, .v = top_v2, .light = 15, .side = 0},
                                mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = top_u2, .v = top_v2, .light = 15, .side = 0},
                                mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = top_u2, .v = top_v1, .light = 15, .side = 0},
                                true);
                        } else if (y2 >= 16 && (chy2 == nullptr || !chy2->GetBlockAt(x1, y2-15, z1).m_Opaque)) {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = top_u1, .v = top_v1, .light = 15, .side = 0},
                                mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = top_u1, .v = top_v2, .light = 15, .side = 0},
                                mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = top_u2, .v = top_v2, .light = 15, .side = 0},
                                mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = top_u2, .v = top_v1, .light = 15, .side = 0},
                                true);
                        }

                        //Bottom
                        if (y1 > 0 && !to_build->GetBlockAt(x1, y1 - 1, z1).m_Opaque)
                        {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = bott_u1, .v = bott_v1, .light = 15, .side = 5},
                                mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = bott_u1, .v = bott_v2, .light = 15, .side = 5},
                                mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = bott_u2, .v = bott_v2, .light = 15, .side = 5},
                                mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = bott_u2, .v = bott_v1, .light = 15, .side = 5},
                                false);
                        } else if (y1 <= 0 && (chy1 == nullptr || !chy1->GetBlockAt(x1, y1+15, z1).m_Opaque)) {
                            mb.AddFace(
                                mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = bott_u1, .v = bott_v1, .light = 15, .side = 5},
                                mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = bott_u1, .v = bott_v2, .light = 15, .side = 5},
                                mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = bott_u2, .v = bott_v2, .light = 15, .side = 5},
                                mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = bott_u2, .v = bott_v1, .light = 15, .side = 5},
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

    void World::HideChunk(ChunkCoord pos)
    {
        this->GetChunk(pos)->Hide();
    }

    std::deque<Chunk> World::GetChunks()
    {
        return this->m_Chunks;
    }
} // namespace world
