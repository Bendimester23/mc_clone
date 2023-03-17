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

#define MAX_UPLOADS_PER_FRAME 500

namespace world
{
    class World {
        std::deque<ChunkCoord> m_GeneratedChunks;
        std::list<Chunk*> m_UploadQueue;
        std::list<Chunk*> m_BuildQueue;
        std::list<ChunkCoord> m_GenerateQueue;
        std::thread m_ChunkBuildThread;
        std::timed_mutex m_BuildQueueLock;

    public:
        ChunkCoord m_CurrPos{0, 0, 0};
        bool m_NeedsRecheck = false;
        std::deque<Chunk> m_Chunks;
        int chunkUploads = 0;

        void Clear();

        void ChunkBuildWorker(bool* running);

        void ChunkGenerateWorker(const bool* running);

        void Update(double delta);

        Chunk* GetChunk(ChunkCoord pos);

        void RebuildAll();

        void GenerateChunk(ChunkCoord pos);

        void QueueBuildJob(ChunkCoord pos);

        std::deque<Chunk> GetChunks() const;

        void HideChunk(ChunkCoord pos);

        bool HasChunk(ChunkCoord pos);

        int GenerateQueueLength();

        int BuildQueueLength();
    };
} // namespace world


#endif //MC_CLONE_2_WORLD_H
