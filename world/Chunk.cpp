//
// Created by bendi on 2022. 07. 05..
//

#include "Chunk.h"

namespace chunk {
    void Chunk::SetBlock(BlockCoordinate blockPos, block::Block block) {
        size_t pos = blockPos.x << 8 | blockPos.y << 4 | blockPos.z;
        this->blocks[pos] = block.m_Id;
        this->SetDirty(true);
    }

    block::Block Chunk::GetBlock(BlockCoordinate blockPos) {
        size_t pos = blockPos.x << 8 | blockPos.y << 4 | blockPos.z;
        return block::GetBlockById(this->blocks[pos]);
    }

    void Chunk::SetDirty(bool dirty) {
        this->m_Dirty = dirty;
    }

    bool Chunk::IsDirty() const {
        return this->m_Dirty;
    }

    Chunk::Chunk(ChunkCoordinate pos): m_Position(0, 0, 0) {
        this->m_Position = pos;
        this->m_Dirty = true;
    }
}
