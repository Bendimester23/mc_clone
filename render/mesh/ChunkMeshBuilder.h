//
// Created by bendi on 2022. 07. 05..
//

#ifndef MC_CLONE_CHUNKMESHBUILDER_H
#define MC_CLONE_CHUNKMESHBUILDER_H


#include <vector>
#include "../../world/types.h"

struct ChunkVertex {
    BlockCoordinate pos = BlockCoordinate(0, 0, 0);
    SimpleUV uv = SimpleUV(0, 0);
    char light;

    ChunkVertex() = default;

    ChunkVertex(char x, char y, char z, char u, char v, char light);
};


//x    y    z    u    v    light
//0000 0000 0000 0000 0000 0000  0000 0000
class ChunkMeshBuilder {
    std::vector<unsigned int> m_Vertices;
    std::vector<unsigned short> m_Indices;

public:
    ChunkMeshBuilder();

    void AddVertex(ChunkVertex vertex);

    unsigned int PackVertex(ChunkVertex vertex);

    void AddQuad(ChunkVertex a, ChunkVertex b, ChunkVertex c, ChunkVertex d, bool cc);

    std::vector<unsigned int> GetVertices();

    std::vector<unsigned short> GetIndices();

    int GetIndicesCount();

    void Trim();
};


#endif //MC_CLONE_CHUNKMESHBUILDER_H
