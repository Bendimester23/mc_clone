#ifndef MC_CLONE_2_WORLD_H
#define MC_CLONE_2_WORLD_H

#include <unordered_map>
#include <thread>
#include <memory>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include "./ChunkCoord.h"
#include "./Chunk.h"
#include "../../utils/TaskQueue.h"

namespace world
{
    class World {
        //TODO fix this thing
        std::unordered_map<ChunkCoord, Chunk> m_Chunks;
        utils::TaskQueue<ChunkCoord> m_BuildQueue;
        utils::TaskQueue<ChunkCoord> m_UploadQueue;
        std::thread m_ChunkBuildThread;

    public:
        void Clear();

        void ChunkBuildWorker(bool* running);

        void Update(double delta);

        void GenerateChunk(ChunkCoord pos);
    };
} // namespace world


#endif //MC_CLONE_2_WORLD_H