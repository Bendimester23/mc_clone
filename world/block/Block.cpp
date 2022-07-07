//
// Created by bendi on 2022. 07. 07..
//

#include "Block.h"

namespace block {
    bool Block::IsOpaque() {
        return this->m_Type == BlockType::SOLID;
    }

    Block GetBlockById(short id) {
        switch (id) {
            case 1:
                return block::BLOCK_GRASS;
            case 2:
                return block::BLOCK_DIRT;

            case 3:
                return block::BLOCK_STONE;

            default:
                return block::BLOCK_AIR;
        }
    }
}
