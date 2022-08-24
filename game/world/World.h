#ifndef MC_CLONE_2_WORLD_H
#define MC_CLONE_2_WORLD_H

#include <unordered_map>
#include <deque>
#include <thread>
#include <memory>
#include <list>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include "./ChunkCoord.h"
#include "./Chunk.h"
#include "../mesh/ChunkMeshBuilder.h"

namespace world
{
    class World {
        std::deque<Chunk> m_Chunks;
        std::list<Chunk*> m_UploadQueue;
        std::list<Chunk*> m_BuildQueue;
        std::list<ChunkCoord> m_GenerateQueue;
        std::thread m_ChunkBuildThread;

    public:
        void Clear();

        void ChunkBuildWorker(bool* running);

        void ChunkGenerateWorker(bool* running);

        void Update(double delta);

        Chunk* GetChunk(ChunkCoord pos);

        void RebuildAll();

        void GenerateChunk(ChunkCoord pos);

        std::deque<Chunk> GetChunks();

        void HideChunk(ChunkCoord pos);
    };
} // namespace world


#endif //MC_CLONE_2_WORLD_H
