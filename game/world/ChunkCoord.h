#ifndef MC_CLONE_2_CHUNKCOORD_H
#define MC_CLONE_2_CHUNKCOORD_H

#include <functional>

namespace world {
    struct ChunkCoord {
    public:
        int x;
        int y;
        int z;

        bool operator==(ChunkCoord other);

        ChunkCoord operator+(ChunkCoord other);

        int squaredDst(ChunkCoord other) const;

        int dstFromOrigin() const;

        int maxDst(ChunkCoord other) const;
    };

    struct HeightMapCoord {
    public:
        int x;
        int z;

        bool operator==(HeightMapCoord other) const;
    };
} // namespace world

#endif // MC_CLONE_2_CHUNKCOORD_H
