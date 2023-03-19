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
#include "./HeightMap.hpp"

#define MAX_UPLOADS_PER_FRAME 500

#define SEED 69420
#define HEIGHT_OCTAVES 4
#define OCTAVES 3

namespace world {
    class World {
        std::deque<ChunkCoord> m_GeneratedChunks;
        std::list<Chunk *> m_UploadQueue;
        std::list<Chunk *> m_BuildQueue;
        std::list<ChunkCoord> m_GenerateQueue;
        std::deque<HeightMap> m_Heightmaps;
        std::thread m_ChunkBuildThread;
        std::timed_mutex m_BuildQueueLock;

    public:
        ChunkCoord m_CurrPos{0, 0, 0};
        ChunkCoord m_PrevPos{0, 0, 0};
        bool m_NeedsRecheck = false;
        std::deque<Chunk> m_Chunks;
        std::timed_mutex m_CurrPosMutex;
        std::timed_mutex m_ChunksMutex;
        int chunkUploads = 0;

        void Clear();

        void ChunkBuildWorker(bool *running);

        void ChunkGenerateWorker(const bool *running);

        void Update(double delta);

        Chunk *GetChunk(ChunkCoord pos);

        void RebuildAll();

        void GenerateChunk(ChunkCoord pos);

        void QueueBuildJob(ChunkCoord pos);

        const std::deque<Chunk>& GetChunks() const;

        void HideChunk(ChunkCoord pos);

        bool HasChunk(ChunkCoord pos);

        int GenerateQueueLength();

        int BuildQueueLength();

        static float Sample2DNoise(float x, float y);
        static float Sample2DOctaveNoise(float x, float y, char octaves = 1);

        //Samples multiple noises and returns a value from -1 to 1
        static float SampleMultiNoise(const HeightMap& height, int x, int y, int z);

        HeightMap GetHeightMap(int x, int z);

        HeightMap GenerateHeightMap(int x, int z);

        static float GetHeight(int x, int z);
    };
} // namespace world


#endif //MC_CLONE_2_WORLD_H
