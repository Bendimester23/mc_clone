#ifndef MC_CLONE_2_CONTENT_H
#define MC_CLONE_2_CONTENT_H

#include "./Block.h"

namespace block {
    static Block content[] {
    {
        0,
        "air",
        {0, 0},
        {0, 0},
        {0, 0},
        false,
    }, {
        1,
        "stone",
        {3, 0},
        {3, 0},
        {3, 0},
        true
    }, {
        2,
        "dirt",
        {4, 0},
        {4, 0},
        {4, 0},
        true
    }, {
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
