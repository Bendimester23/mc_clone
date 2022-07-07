//
// Created by bendi on 2022. 07. 05..
//

#include "Chunk.h"

std::string ChunkCoordinate::to_string() const {
    return "ChunkCoordinate[x" + std::to_string(x) + "; y=" + std::to_string(y) + "; z=" + std::to_string(z);
}

ChunkCoordinate::ChunkCoordinate(int pX, int pY, int pZ) {
    this->x = pX;
    this->y = pY;
    this->z = pZ;
}

ChunkCoordinate::ChunkCoordinate() = default;

std::string BlockCoordinate::to_string() const {
    return "BlockCoordinate[x" + std::to_string(x) + "; y=" + std::to_string(y) + "; z=" + std::to_string(z);
}

BlockCoordinate::BlockCoordinate(char pX, char pY, char pZ) {
    this->x = pX;
    this->y = pY;
    this->z = pZ;
}

void Chunk::SetBlock(BlockCoordinate blockPos, char block) {
    size_t pos = blockPos.x << 8 | blockPos.y << 4 | blockPos.z;
    this->blocks[pos] = block;
    this->SetDirty(true);
}

char Chunk::GetBlock(BlockCoordinate blockPos) {
    size_t pos = blockPos.x << 8 | blockPos.y << 4 | blockPos.z;
    return this->blocks[pos];
}

void Chunk::SetDirty(bool dirty) {
    this->m_Dirty = dirty;
}

bool Chunk::IsDirty() const {
    return this->m_Dirty;
}

Chunk::Chunk(ChunkCoordinate pos) {
    this->m_Position = pos;
    this->m_Dirty = true;
}
