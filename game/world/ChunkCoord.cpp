#include "./ChunkCoord.h"

namespace world
{
    bool ChunkCoord::operator== (ChunkCoord other) {
        return x == other.x && y == other.y && z == other.z;
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
