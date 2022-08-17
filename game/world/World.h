#ifndef MC_CLONE_2_WORLD_H
#define MC_CLONE_2_WORLD_H

#include <unordered_map>
#include "./ChunkCoord.h"
#include "./Chunk.h"

namespace world
{
    class World {
        std::unordered_map<ChunkCoord, Chunk> m_Chunks;
    };
} // namespace world


#endif //MC_CLONE_2_WORLD_H