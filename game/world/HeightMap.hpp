//
// Created by bendi on 3/18/23.
//

#ifndef MC_CLONE_2_HEIGHTMAP_HPP
#define MC_CLONE_2_HEIGHTMAP_HPP

#include <array>

namespace world {
    struct HeightMap {
    public:
        int x;
        int z;
        std::array<std::array<int, 16>, 16> data;

        HeightMap& operator= (const HeightMap &other);
    };
}

#endif //MC_CLONE_2_HEIGHTMAP_HPP
