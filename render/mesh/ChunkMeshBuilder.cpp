//
// Created by bendi on 2022. 07. 05..
//

#include "ChunkMeshBuilder.h"

//x     y     z     u    v    light
//00000 00000 00000 0000 0000 0000  00000
ChunkMeshBuilder::ChunkMeshBuilder() {
    this->m_Vertices.reserve(16 * 16 * 16 * 4);
    this->m_Indices.reserve(16 * 16 * 16 * 4);
}

unsigned int ChunkMeshBuilder::PackVertex(ChunkVertex vertex) {
    unsigned int data = 0;
    data |= (vertex.pos.x & 31) << 27;
    data |= (vertex.pos.y & 31) << 22;
    data |= (vertex.pos.z & 31) << 17;

    data |= (vertex.uv.u & 15) << 13;
    data |= (vertex.uv.v & 15) << 9;

    data |= (vertex.light & 15) << 5;
    if (vertex.pos.x == 16 || vertex.pos.z == 16) {
        int i = 0;
    }
    return data;
}

void ChunkMeshBuilder::AddVertex(ChunkVertex vertex) {
    unsigned int vert = this->PackVertex(vertex);
    for (int i = 0; i < this->m_Indices.size(); i++) {
        if (this->m_Indices[i] == vert) {
            this->m_Indices.push_back(i);
            return;
        }
    }
    this->m_Vertices.push_back(vert);
    this->m_Indices.push_back(this->m_Vertices.size()-1);
}

std::vector<unsigned int> ChunkMeshBuilder::GetVertices() {
    return this->m_Vertices;
}

std::vector<unsigned short> ChunkMeshBuilder::GetIndices() {
    return this->m_Indices;
}

int ChunkMeshBuilder::GetIndicesCount() {
    return (int)this->m_Indices.size();
}

void ChunkMeshBuilder::Trim() {
    this->m_Vertices.shrink_to_fit();
    this->m_Indices.shrink_to_fit();
}

void ChunkMeshBuilder::AddQuad(ChunkVertex a, ChunkVertex b, ChunkVertex c, ChunkVertex d, bool cc) {
    if (cc) {
        this->AddVertex(a);
        this->AddVertex(b);
        this->AddVertex(c);

        this->AddVertex(a);
        this->AddVertex(c);
        this->AddVertex(d);
    } else {
        this->AddVertex(b);
        this->AddVertex(a);
        this->AddVertex(c);

        this->AddVertex(c);
        this->AddVertex(a);
        this->AddVertex(d);
    }
}

ChunkVertex::ChunkVertex(char px, char py, char pz, char pu, char pv, char plight) {
    this->pos = BlockCoordinate(px, py, pz);
    this->uv = SimpleUV(pu, pv);
    this->light = plight;
}
