//
// Created by bendi on 2022. 07. 05..
//

#ifndef MC_CLONE_CHUNK_H
#define MC_CLONE_CHUNK_H

#include <string>

struct ChunkCoordinate {
    ChunkCoordinate(int pX, int pY, int pZ);

    ChunkCoordinate();

    int x, y, z;

    std::string to_string() const;
};

struct BlockCoordinate {
    BlockCoordinate(char pX, char pY, char pZ);

    char x, y, z;

    std::string to_string() const;
};

struct Chunk {
    ChunkCoordinate m_Position;
    char blocks[16*16*16];
    bool m_Dirty;

public:
    Chunk(ChunkCoordinate pos);

    void SetBlock(BlockCoordinate blockPos, char block);

    char GetBlock(BlockCoordinate blockPos);

    void SetDirty(bool dirty);

    bool IsDirty() const;
};


#endif //MC_CLONE_CHUNK_H
