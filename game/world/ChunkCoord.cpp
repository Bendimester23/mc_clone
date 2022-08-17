#include "./ChunkCoord.h"

namespace world
{
    bool ChunkCoord::operator== (const ChunkCoord &other) {
        return x == other.x && y == other.y && z == other.z;
    }
} // namespace world
