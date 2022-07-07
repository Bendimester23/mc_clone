//
// Created by bendi on 2022. 07. 05..
//

#pragma once
#ifndef MC_CLONE_CHUNK_H
#define MC_CLONE_CHUNK_H

#include "block/Block.h"
#include "./types.h"

namespace chunk {
    using namespace block;
    struct Chunk {
        ChunkCoordinate m_Position;
        short blocks[16 * 16 * 16];
        bool m_Dirty;

    public:
        Chunk(ChunkCoordinate pos);

        void SetBlock(BlockCoordinate blockPos, Block bl);

        Block GetBlock(BlockCoordinate blockPos);

        void SetDirty(bool dirty);

        bool IsDirty() const;
    };
}

#endif //MC_CLONE_CHUNK_H
