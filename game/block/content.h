#ifndef MC_CLONE_2_CONTENT_H
#define MC_CLONE_2_CONTENT_H

#include "./Block.h"

#define BLOCK_AIR 0
#define BLOCK_STONE 1
#define BLOCK_DIRT 2
#define BLOCK_GRASS 3

namespace block {
    static Block content[]{
            {
                    0,
                    "air",
                    {0, 0},
                    {0, 0},
                    {0, 0},
                    false,
            },
            {
                    1,
                    "stone",
                    {3, 0},
                    {3, 0},
                    {3, 0},
                    true
            },
            {
                    2,
                    "dirt",
                    {4, 0},
                    {4, 0},
                    {4, 0},
                    true
            },
            {
                    3,
                    "grass_block",
                    {1, 0},
                    {2, 0},
                    {4, 0},
                    true
            }
    };

    Block byId(char id);
}

#endif
