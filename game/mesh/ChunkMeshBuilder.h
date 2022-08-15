//
// Created by bendi on 2022.08.14..
//

#ifndef MC_CLONE_2_CHUNKMESHBUILDER_H
#define MC_CLONE_2_CHUNKMESHBUILDER_H

#include <vector>
#include "VertexData.h"

namespace mesh {

    class ChunkMeshBuilder {
        std::vector<unsigned int> m_Vertices;
        std::vector<unsigned short> m_Indices;

        static unsigned int PackVertex(VertexData v);

    public:
        ChunkMeshBuilder();

        ~ChunkMeshBuilder();

        unsigned short AddVertex(VertexData data);

        void AddFace(VertexData v1, VertexData v2, VertexData v3, VertexData v4, bool cc);

        std::vector<unsigned int> GetVertices();

        std::vector<unsigned short> GetIndices();
    };

} // mesh

#endif //MC_CLONE_2_CHUNKMESHBUILDER_H
