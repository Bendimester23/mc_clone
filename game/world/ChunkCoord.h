#ifndef MC_CLONE_2_CHUNKCOORD_H
#define MC_CLONE_2_CHUNKCOORD_H

#include <functional>

namespace world
{
    struct ChunkCoord
    {
    public:
        int x;
        int y;
        int z;

        bool operator==(ChunkCoord other);

        ChunkCoord operator+(ChunkCoord other);

        float squaredDst(ChunkCoord other) const;

        float dstFromOrigin() const;

        int maxDst(ChunkCoord other);
    };
} // namespace world

#endif // MC_CLONE_2_CHUNKCOORD_H
