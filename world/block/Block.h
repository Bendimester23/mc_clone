//
// Created by bendi on 2022. 07. 07..
//

#pragma once
#ifndef MC_CLONE_BLOCK_H
#define MC_CLONE_BLOCK_H

#include "../types.h"

namespace block {
    enum BlockType {
        SOLID,
        TRANSPARENT,
        VEGETATION,
        SOLID_MODEL,
        FLUID,
        NONE
    };

    struct Block {
    public:
        short m_Id;
        std::string name;
        SimpleUV m_TopTexture;
        SimpleUV m_FrontTexture;
        SimpleUV m_SideTexture;
        SimpleUV m_BottomTexture;
        BlockType m_Type;

        bool IsOpaque();
    };

    static Block BLOCK_AIR = Block{
            0,
            "air",
            SimpleUV(0, 0),
            SimpleUV(0, 0),
            SimpleUV(0, 0),
            SimpleUV(0, 0),
            BlockType::NONE
    };

    static Block BLOCK_GRASS = Block{
            1,
            "grass",
            SimpleUV(1, 0),
            SimpleUV(2, 0),
            SimpleUV(2, 0),
            SimpleUV(4, 0),
            BlockType::SOLID
    };

    static Block BLOCK_DIRT = Block{
            2,
            "dirt",
            SimpleUV(4, 0),
            SimpleUV(4, 0),
            SimpleUV(4, 0),
            SimpleUV(4, 0),
            BlockType::SOLID
    };

    static Block BLOCK_STONE = Block {
        3,
        "stone",
        SimpleUV(3, 0),
        SimpleUV(3, 0),
        SimpleUV(3, 0),
        SimpleUV(3, 0),
        BlockType::SOLID
    };

    Block GetBlockById(short id);
}

#endif //MC_CLONE_BLOCK_H
