#include "./World.h"
#include "../block/content.h"

namespace world {
    using namespace std::chrono_literals;

    void World::Clear() {
        this->m_Chunks.clear();
        this->m_BuildQueue.clear();
        this->m_UploadQueue.clear();
    }

    void World::Update(double delta) {
        if (this->m_BuildQueueLock.try_lock_for(2ms)) {
            short count = 0;
            while (!this->m_UploadQueue.empty()) {
                auto to_upload = this->m_UploadQueue.front();
                if (to_upload == nullptr) {
                    this->m_UploadQueue.pop_front();
                    continue;
                }
                to_upload->Upload();
                this->m_UploadQueue.pop_front();
                count++;
                if (count >= MAX_UPLOADS_PER_FRAME) {
                    //Don't upload all the chunks in one frame, because there can be a lot
                    break;
                }
            }
            this->chunkUploads = count;
//            this->m_UploadQueue.clear();
            this->m_BuildQueueLock.unlock();
        } else {
            this->chunkUploads = 0;
        }
//
//        auto maxDst = 6 * 6;
//
//        if (this->m_CurrPosMutex.try_lock_for(2ms)) {
//            auto pos = m_CurrPos;
//            this->m_CurrPosMutex.unlock();
//            if (this->m_ChunksMutex.try_lock_for(2ms)) {
//                for (int i = 0; i < this->m_Chunks.size(); ++i) {
//                    if (this->m_Chunks[i].GetPosition().squaredDst(pos) > maxDst) {
//                        this->m_Chunks.erase(this->m_Chunks.begin()+i);
//                    }
//                }
//                this->m_ChunksMutex.unlock();
//            }
//        }
    }

    void World::GenerateChunk(ChunkCoord pos) {
        this->m_GenerateQueue.emplace_back(pos);
    }

    Chunk *World::GetChunk(ChunkCoord pos) {
        for (auto &e: this->m_Chunks) {
            if (e.GetPosition() == pos)
                return &e;
        }
        return nullptr;
    }

    void World::QueueBuildJob(ChunkCoord pos) {
        for (const auto &item: this->m_BuildQueue) {
            if (item == nullptr) {
                //this->m_BuildQueue.emplace_back(this->GetChunk(pos));
                continue;
            }
            if (pos == item->GetPosition()) return;
        }
        this->m_ChunksMutex.lock();
        this->m_BuildQueue.emplace_back(this->GetChunk(pos));
        this->m_ChunksMutex.unlock();
    }

    void World::ChunkGenerateWorker(const bool *running) {
        while (*running) {
            while (this->m_GenerateQueue.empty() && *running) {
                this->m_CurrPosMutex.lock();
                auto pos = m_CurrPos;
                this->m_CurrPosMutex.unlock();
                if (this->m_CurrPos == this->m_PrevPos) {
                    std::this_thread::sleep_for(100us);
                    break;
                }
                this->m_PrevPos = this->m_CurrPos;
                char viewDst = 4;
                std::vector<world::ChunkCoord> toGenerate;
                //Reserve some memory in order to not get slowed down by continuous allocations and moves
                toGenerate.reserve(viewDst * viewDst * viewDst);

                for (int x = -viewDst; x < viewDst; ++x) {
                    for (int y = -viewDst; y < viewDst; ++y) {
                        for (int z = -viewDst; z < viewDst; ++z) {
                            if (!this->HasChunk(m_CurrPos + world::ChunkCoord{x, y, z})) {
                                toGenerate.emplace_back(world::ChunkCoord{x, y, z});
                            }
                        }
                    }
                }
                spdlog::info("Generating {} chunks around: {} {} {}", toGenerate.size(), m_CurrPos.x, m_CurrPos.y,
                             m_CurrPos.z);

                std::sort(toGenerate.begin(), toGenerate.end(), [this](auto a, auto b) {
                    return a.squaredDst(m_CurrPos) < b.squaredDst(m_CurrPos);
                });

                //TODO only rebuild chunks once all of them are generated in order to reduce rebuilds by 8/9
                for (auto &item: toGenerate) {
                    this->GenerateChunk(item + m_CurrPos);
                }
//                this->m_NeedsRecheck = false;
            }
            if (!(*running)) break;

            if (this->m_GenerateQueue.empty()) continue;

            auto pos = this->m_GenerateQueue.front();

//            if (pos.maxDst(m_CurrPos) > 150) {
//                this->m_GenerateQueue.pop_front();
//                continue;
//            }

            auto ch = Chunk(pos);
            auto height = GetHeightMap(pos.x, pos.z);
            if (height.x == 69420 && height.z == 69420) {
                height = GenerateHeightMap(pos.x, pos.z);
            }
            for (char x = 0; x < 16; x++) {
                for (char z = 0; z < 16; z++) {
//                    auto height = glm::simplex(glm::vec2((pos.x * 16 + x) * 0.03f, (pos.z * 16 + z) * 0.03f)) * 10 + 20;
//                    height -= pos.y * 16;
//                    for (char y = 0; y < glm::min(height, 16.0f); y++) {
//                        if (y == glm::ceil(height) - 1) ch.SetBlockAt(x, y, z, 3);
//                        else if (glm::floor(height) - y < 4) ch.SetBlockAt(x, y, z, 2);
//                        else ch.SetBlockAt(x, y, z, 1);
//                    }
                    //TODO fix worldgen
                    for (char y = 15; y >= 0; --y) {
                        auto val = World::SampleMultiNoise(height, x+pos.x*16, y+pos.y*16, z+pos.z*16);
                        if (val > 0.5f) {
                            if (height.data[x][z] == y+pos.y*16) ch.SetBlockAt(x, y, z, BLOCK_GRASS);
                            else if (height.data[x][z] - 1 == y+pos.y*16) ch.SetBlockAt(x, y, z, BLOCK_DIRT);
                            else if (height.data[x][z] - 2 == y+pos.y*16) ch.SetBlockAt(x, y, z, BLOCK_DIRT);
                            else ch.SetBlockAt(x, y, z, BLOCK_STONE);
                        }
                    }
                }
            }

            this->m_GeneratedChunks.emplace_back(pos);
            this->m_ChunksMutex.lock();
            this->m_Chunks.push_back(ch);
            this->m_ChunksMutex.unlock();
            {
                std::lock_guard<std::timed_mutex> guard(this->m_BuildQueueLock);
                this->QueueBuildJob(this->m_Chunks.back().GetPosition());
                this->QueueBuildJob(pos + ChunkCoord{1, 0, 0});
                this->QueueBuildJob(pos + ChunkCoord{-1, 0, 0});
                this->QueueBuildJob(pos + ChunkCoord{0, 1, 0});
                this->QueueBuildJob(pos + ChunkCoord{0, -1, 0});
                this->QueueBuildJob(pos + ChunkCoord{0, 0, 1});
                this->QueueBuildJob(pos + ChunkCoord{0, 0, -1});
            }
            this->m_GenerateQueue.pop_front();
        }
    }

    void World::RebuildAll() {
        std::lock_guard<std::timed_mutex> guard(this->m_BuildQueueLock);
        for (auto &e: this->m_Chunks) {
            this->m_BuildQueue.push_back(&e);
        }
    }

    void World::ChunkBuildWorker(bool *running) {
        while (*running) {

            while (this->m_BuildQueue.empty() && *running) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            if (!(*running))
                break;

//            this->m_BuildQueueLock.lock();

            if (!this->m_BuildQueueLock.try_lock_for(25ms)) {
                std::this_thread::sleep_for(20ms);
                return;
            }

            this->m_ChunksMutex.lock();
            auto to_build = this->m_BuildQueue.front();

            if (to_build == nullptr) {
                this->m_BuildQueue.pop_front();
                this->m_BuildQueueLock.unlock();
                this->m_ChunksMutex.unlock();
                continue;
            }


            mesh::ChunkMeshBuilder mb;

            auto thisPos = to_build->GetPosition();

            auto chy1 = this->GetChunk(thisPos + ChunkCoord{0, -1, 0});
            auto chy2 = this->GetChunk(thisPos + ChunkCoord{0, 1, 0});
            auto chx1 = this->GetChunk(thisPos + ChunkCoord{-1, 0, 0});
            auto chx2 = this->GetChunk(thisPos + ChunkCoord{1, 0, 0});
            auto chz1 = this->GetChunk(thisPos + ChunkCoord{0, 0, -1});
            auto chz2 = this->GetChunk(thisPos + ChunkCoord{0, 0, 1});

            for (char x1 = 0; x1 < 16; x1++) {
                for (char y1 = 0; y1 < 16; y1++) {
                    for (char z1 = 0; z1 < 16; z1++) {
                        char x2 = static_cast<char>(x1 + 1);
                        char y2 = static_cast<char>(y1 + 1);
                        char z2 = static_cast<char>(z1 + 1);

                        auto curr_b = to_build->GetBlockAt(x1, y1, z1);

                        if (!curr_b.m_Opaque) continue;


                        char side_u1 = curr_b.m_Side.u;
                        char side_u2 = curr_b.m_Side.u + 1;
                        char side_v1 = curr_b.m_Side.v;
                        char side_v2 = curr_b.m_Side.v + 1;

                        char bott_u1 = curr_b.m_Bottom.u;
                        char bott_u2 = curr_b.m_Bottom.u + 1;
                        char bott_v1 = curr_b.m_Bottom.v;
                        char bott_v2 = curr_b.m_Bottom.v + 1;

                        char top_u1 = curr_b.m_Top.u;
                        char top_u2 = curr_b.m_Top.u + 1;
                        char top_v1 = curr_b.m_Top.v;
                        char top_v2 = curr_b.m_Top.v + 1;

                        //X+
                        if (x2 < 16 && !to_build->GetBlockAt(x1 + 1, y1, z1).m_Opaque) {
                            mb.AddFace(
                                    mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = side_u1, .v = side_v2, .light = 15, .side = 3},
                                    mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = side_u2, .v = side_v2, .light = 15, .side = 3},
                                    mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = side_u2, .v = side_v1, .light = 15, .side = 3},
                                    mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = side_u1, .v = side_v1, .light = 15, .side = 3},
                                    false);
                        } else if (x2 >= 16 && (chx2 == nullptr || !chx2->GetBlockAt(x2 - 15, y1, z1).m_Opaque)) {
                            mb.AddFace(
                                    mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = side_u1, .v = side_v2, .light = 15, .side = 3},
                                    mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = side_u2, .v = side_v2, .light = 15, .side = 3},
                                    mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = side_u2, .v = side_v1, .light = 15, .side = 3},
                                    mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = side_u1, .v = side_v1, .light = 15, .side = 3},
                                    false);
                        }

                        //X-
                        if (x1 > 0 && !to_build->GetBlockAt(x1 - 1, y1, z1).m_Opaque) {
                            mb.AddFace(
                                    mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = side_u1, .v = side_v2, .light = 15, .side = 3},
                                    mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = side_u2, .v = side_v2, .light = 15, .side = 3},
                                    mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = side_u2, .v = side_v1, .light = 15, .side = 3},
                                    mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = side_u1, .v = side_v1, .light = 15, .side = 3},
                                    true);
                        } else if (x1 <= 0 && (chx1 == nullptr || !chx1->GetBlockAt(x1 + 15, y1, z1).m_Opaque)) {
                            mb.AddFace(
                                    mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = side_u1, .v = side_v2, .light = 15, .side = 3},
                                    mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = side_u2, .v = side_v2, .light = 15, .side = 3},
                                    mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = side_u2, .v = side_v1, .light = 15, .side = 3},
                                    mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = side_u1, .v = side_v1, .light = 15, .side = 3},
                                    true);
                        }

                        //Z+
                        if (z2 < 16 && !to_build->GetBlockAt(x1, y1, z1 + 1).m_Opaque) {
                            mb.AddFace(
                                    mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = side_u1, .v = side_v2, .light = 15, .side = 4},
                                    mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = side_u1, .v = side_v1, .light = 15, .side = 4},
                                    mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = side_u2, .v = side_v1, .light = 15, .side = 4},
                                    mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = side_u2, .v = side_v2, .light = 15, .side = 4},
                                    false);
                        } else if (z2 >= 16 && (chz2 == nullptr || !chz2->GetBlockAt(x1, y1, z2 - 15).m_Opaque)) {
                            mb.AddFace(
                                    mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = side_u1, .v = side_v2, .light = 15, .side = 4},
                                    mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = side_u1, .v = side_v1, .light = 15, .side = 4},
                                    mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = side_u2, .v = side_v1, .light = 15, .side = 4},
                                    mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = side_u2, .v = side_v2, .light = 15, .side = 4},
                                    false);
                        }

                        //Z-
                        if (z1 > 0 && !to_build->GetBlockAt(x1, y1, z1 - 1).m_Opaque) {
                            mb.AddFace(
                                    mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = side_u1, .v = side_v2, .light = 15, .side = 4},
                                    mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = side_u1, .v = side_v1, .light = 15, .side = 4},
                                    mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = side_u2, .v = side_v1, .light = 15, .side = 4},
                                    mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = side_u2, .v = side_v2, .light = 15, .side = 4},
                                    true);
                        } else if (z1 <= 0 && (chz1 == nullptr || !chz1->GetBlockAt(x1, y1, z1 + 15).m_Opaque)) {
                            mb.AddFace(
                                    mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = side_u1, .v = side_v2, .light = 15, .side = 4},
                                    mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = side_u1, .v = side_v1, .light = 15, .side = 4},
                                    mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = side_u2, .v = side_v1, .light = 15, .side = 4},
                                    mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = side_u2, .v = side_v2, .light = 15, .side = 4},
                                    true);
                        }

                        //Top
                        if (y2 < 16 && !to_build->GetBlockAt(x1, y1 + 1, z1).m_Opaque) {
                            mb.AddFace(
                                    mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = top_u1, .v = top_v1, .light = 15, .side = 0},
                                    mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = top_u1, .v = top_v2, .light = 15, .side = 0},
                                    mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = top_u2, .v = top_v2, .light = 15, .side = 0},
                                    mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = top_u2, .v = top_v1, .light = 15, .side = 0},
                                    true);
                        } else if (y2 >= 16 && (chy2 == nullptr || !chy2->GetBlockAt(x1, y2 - 15, z1).m_Opaque)) {
                            mb.AddFace(
                                    mesh::VertexData{.x = x1, .y = y2, .z = z1, .u = top_u1, .v = top_v1, .light = 15, .side = 0},
                                    mesh::VertexData{.x = x1, .y = y2, .z = z2, .u = top_u1, .v = top_v2, .light = 15, .side = 0},
                                    mesh::VertexData{.x = x2, .y = y2, .z = z2, .u = top_u2, .v = top_v2, .light = 15, .side = 0},
                                    mesh::VertexData{.x = x2, .y = y2, .z = z1, .u = top_u2, .v = top_v1, .light = 15, .side = 0},
                                    true);
                        }

                        //Bottom
                        if (y1 > 0 && !to_build->GetBlockAt(x1, y1 - 1, z1).m_Opaque) {
                            mb.AddFace(
                                    mesh::VertexData{.x = x1, .y = y1, .z = z1, .u = bott_u1, .v = bott_v1, .light = 15, .side = 5},
                                    mesh::VertexData{.x = x1, .y = y1, .z = z2, .u = bott_u1, .v = bott_v2, .light = 15, .side = 5},
                                    mesh::VertexData{.x = x2, .y = y1, .z = z2, .u = bott_u2, .v = bott_v2, .light = 15, .side = 5},
                                    mesh::VertexData{.x = x2, .y = y1, .z = z1, .u = bott_u2, .v = bott_v1, .light = 15, .side = 5},
                                    false);
                        } else if (y1 <= 0 && (chy1 == nullptr || !chy1->GetBlockAt(x1, y1 + 15, z1).m_Opaque)) {
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

            this->m_UploadQueue.push_back(to_build);
            this->m_BuildQueue.pop_front();
            this->m_ChunksMutex.unlock();
            this->m_BuildQueueLock.unlock();
        }
    }

    void World::HideChunk(ChunkCoord pos) {
        this->GetChunk(pos)->Hide();
    }

    const std::deque<Chunk>& World::GetChunks() const {
        return this->m_Chunks;
    }

    bool World::HasChunk(ChunkCoord pos) {
        for (const auto &item: this->m_GeneratedChunks) {
            if (pos == item) {
                return true;
            }
        }
        return false;
    }

    int World::GenerateQueueLength() {
        return static_cast<int>(this->m_GenerateQueue.size());
    }

    int World::BuildQueueLength() {
        return static_cast<int>(this->m_BuildQueue.size());
    }

    float World::SampleMultiNoise(const HeightMap& height, int x, int y, int z) {
        auto height_value = height.data[x & 0xF][z & 0xF];

        if (y > height_value) return 0.0f;//glm::max(0.0f, y - height_value);

//        auto nearSurface = glm::min((static_cast<float>(height_value - y) / 8.0f) * 0.7f, 1.0f);

        return glm::simplex(glm::vec3(static_cast<float>(x) / 48.0f, static_cast<float>(y) / 48.0f, static_cast<float>(z) / 48.0f)) + 0.85f;// + (1.15f * (0.8f - nearSurface));
    }

    float World::Sample2DNoise(float x, float y) {
        return glm::simplex(glm::vec2(x + SEED, y - SEED));
    }

    float World::Sample2DOctaveNoise(float x, float y, char octaves) {
        float val = 0.0f;
        for (int i = 0; i < octaves; ++i) {
            val += World::Sample2DNoise(x * static_cast<float>(1 << i), y * static_cast<float>(1 << i)) / static_cast<float>(1 << i);
        }
        return val;
    }

    HeightMap World::GetHeightMap(int x, int z) {
        static auto null = HeightMap{x = 69420, z = 69420};
        for (auto &item: this->m_Heightmaps) {
            if (item.x == x && item.z == z) {
                return item;
            }
        }
        return null;
    }

    HeightMap World::GenerateHeightMap(int x, int z) {
        auto val = HeightMap{
            .x = x,
            .z = z,
            .data = std::array<std::array<int, 16>, 16>(),
        };
        for (int bx = 0; bx < 16; ++bx) {
            val.data[bx] = std::array<int, 16>();
            for (int bz = 0; bz < 16; ++bz) {
                val.data[bx][bz] = static_cast<int>(GetHeight(x * 16 + bx, z * 16 + bz));
            }
        }
        this->m_Heightmaps.emplace_back(val);
        return val;
    }

    float World::GetHeight(int x, int z) {
        auto pv = World::Sample2DOctaveNoise(static_cast<float>(x) / 512, static_cast<float>(z) / 512, HEIGHT_OCTAVES);

        auto height_raw = World::Sample2DOctaveNoise(static_cast<float>(x) / 32, static_cast<float>(z) / 32, OCTAVES);

        return (height_raw * 4 + (pv * 32)) + 64;
    }
} // namespace world
