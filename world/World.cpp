//
// Created by bendi on 2022. 07. 05..
//

#include "World.h"

void World::LoadChunk(int x, int y, int z) {
    auto chunk = new Chunk(ChunkCoordinate{x, y, z});
    auto mesh = ChunkMesh(chunk);
    this->meshes.push_back(mesh);
}

void World::Render() {
    for (auto &item : this->meshes) {
        item.Bind();

        item.UnBind();
    }
}
