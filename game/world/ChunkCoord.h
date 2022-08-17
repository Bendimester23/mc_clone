#ifndef MC_CLONE_2_CHUNKCOORD_H
#define MC_CLONE_2_CHUNKCOORD_H

namespace world
{
    struct ChunkCoord
    {
    public:
        int x;
        int y;
        int z;

        bool operator==(const ChunkCoord &other);
    };
} // namespace world

#endif // MC_CLONE_2_CHUNKCOORD_H