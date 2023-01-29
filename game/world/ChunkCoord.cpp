#include "./ChunkCoord.h"

namespace world
{
    bool ChunkCoord::operator== (ChunkCoord other) {
        return x == other.x && y == other.y && z == other.z;
    }

    ChunkCoord ChunkCoord::operator+(world::ChunkCoord other)
    {
        return ChunkCoord {this->x + other.x, this->y + other.y, this->z + other.z};
    }

    float ChunkCoord::squaredDst(world::ChunkCoord other) const
    {
        float dX = x - other.x;
        float dY = y - other.y;
        float dZ = z - other.z;

        return dX * dX + dY * dY + dZ * dZ;
    }

    float ChunkCoord::dstFromOrigin() const {
        return x*x + y*y + z*z;
    }

    int ChunkCoord::maxDst(world::ChunkCoord other)
    {
        int dX = x - other.x;
        int dY = y - other.y;
        int dZ = z - other.z;
        if (dX < 0) dX *= -1;
        if (dY < 0) dY *= -1;
        if (dZ < 0) dZ *= -1;

        if (dX < dY) {
            if (dZ > dY) return dZ;
            return dY;
        }
        return dX;
    }



} // namespace world

namespace std {
    template<>
    struct hash<world::ChunkCoord>
    {
        std::size_t operator()(const world::ChunkCoord& k) const 
        {
            using std::size_t;
            using std::hash;
            
            return ((hash<int>()(k.x) ^ (hash<int>()(k.y) << 1)) >> 1) ^ (hash<int>()(k.z) << 1);
        }
    };
} //std
