//
// Created by bendi on 2022.08.14..
//

#include "ChunkMeshBuilder.h"

namespace mesh {
    ChunkMeshBuilder::ChunkMeshBuilder(): m_Indices(), m_Vertices() {
        this->m_Indices.reserve(16384);
        this->m_Vertices.reserve(16384);
    }

    ChunkMeshBuilder::~ChunkMeshBuilder() {
        this->m_Vertices.clear();
        this->m_Indices.clear();
    }

    unsigned short ChunkMeshBuilder::AddVertex(VertexData data) {
        this->m_Vertices.push_back(mesh::ChunkMeshBuilder::PackVertex(data));
        return m_Vertices.size()-1;
    }

    unsigned int ChunkMeshBuilder::PackVertex(VertexData v) {
        unsigned int data = 0;
        data |= (v.x & 31) << 27;
        data |= (v.y & 31) << 22;
        data |= (v.z & 31) << 17;

        data |= (v.u & 31) << 12;
        data |= (v.v & 31) << 7;

        data |= (v.light & 15) << 3;

        data |= v.side & 7;
        return data;
    }

    void ChunkMeshBuilder::AddFace(VertexData v1, VertexData v2, VertexData v3, VertexData v4, bool cc) {
        auto i1 = this->AddVertex(v1);
        auto i2 = this->AddVertex(v2);
        auto i3 = this->AddVertex(v3);
        auto i4 = this->AddVertex(v4);

        if (cc) {
            m_Indices.push_back(i1);
            m_Indices.push_back(i2);
            m_Indices.push_back(i3);

            m_Indices.push_back(i4);
            m_Indices.push_back(i1);
            m_Indices.push_back(i3);
        } else {
            m_Indices.push_back(i2);
            m_Indices.push_back(i1);
            m_Indices.push_back(i3);

            m_Indices.push_back(i1);
            m_Indices.push_back(i4);
            m_Indices.push_back(i3);
        }
    }

    std::vector<unsigned int> ChunkMeshBuilder::GetVertices() {
        return this->m_Vertices;
    }

    std::vector<unsigned short> ChunkMeshBuilder::GetIndices() {
        return this->m_Indices;
    }
} // mesh