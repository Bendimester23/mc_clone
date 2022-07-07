//
// Created by bendi on 2022. 07. 05..
//

#ifndef MC_CLONE_WORLD_H
#define MC_CLONE_WORLD_H


#include <vector>
#include "Chunk.h"
#include "../render/mesh/ChunkMesh.h"

class World {
    std::vector<Chunk> chunks;
    std::vector<ChunkMesh> meshes;

    void LoadChunk(int x, int y, int z);

    void Render();
};


#endif //MC_CLONE_WORLD_H
