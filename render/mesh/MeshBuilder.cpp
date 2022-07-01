//
// Created by bendi on 2022. 06. 29..
//

#include "MeshBuilder.h"
#include "spdlog/spdlog.h"

MeshBuilder::MeshBuilder(): m_Indices(), m_Vertices() {
    this->m_Indices.reserve(16000);
    this->m_Vertices.reserve(16000);
}

//Fails when more than one cube is present
void MeshBuilder::AddVertex(VertexCoord vert) {
    unsigned short idx = 0;
    for (const auto &item : this->m_Vertices) {
        if (item.x == vert.x && item.y == vert.y && item.z == vert.z) {
            this->m_Indices.push_back( idx);
            return;
        }
        idx++;
    }
    this->m_Vertices.push_back(vert);
    spdlog::info("Other, slightly different thingy: {}", vert.to_string());
    this->m_Indices.push_back(this->m_Vertices.size()-1);
}

void MeshBuilder::AddQuad(VertexCoord a, VertexCoord b, VertexCoord c, VertexCoord d, bool cc) {
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

std::vector<float> MeshBuilder::GetVertices() {
    std::vector<float> res = std::vector<float>();

    for (auto &item : this->m_Vertices) {
        res.push_back(item.x);
        res.push_back(item.y);
        res.push_back(item.z);
        spdlog::info("Thingy: {}", item.to_string());
    }

    return res;
}

std::vector<unsigned short> MeshBuilder::GetIndices() {
    return this->m_Indices;
}

int MeshBuilder::GetIndicesCount() {
    return (int)this->m_Indices.size();
}

VertexCoord min(VertexCoord a, VertexCoord b) {
    return {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
}

VertexCoord max(VertexCoord a, VertexCoord b) {
    return {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
}

//TODO fails for more than one cube
void MeshBuilder::AddCube(VertexCoord a, VertexCoord b, bool insideOut) {
    auto start = min(a, b);
    auto end = max(a, b);

    //front
    this->AddQuad(
            VertexCoord(start.x, start.y, start.z),
            VertexCoord(start.x, end.y,   start.z),
            VertexCoord(end.x,   end.y,   start.z),
            VertexCoord(end.x,   start.y, start.z),
            !insideOut
    );

    //back
    this->AddQuad(
            VertexCoord(start.x, start.y, end.z),
            VertexCoord(start.x, end.y,   end.z),
            VertexCoord(end.x,   end.y,   end.z),
            VertexCoord(end.x,   start.y, end.z),
            insideOut
    );

    //top
    this->AddQuad(
            VertexCoord(start.x, end.y, start.z),
            VertexCoord(end.x,   end.y, start.z),
            VertexCoord(end.z,   end.y, end.z),
            VertexCoord(start.z, end.y, end.z),
            insideOut
            );

    //bottom
    this->AddQuad(
            VertexCoord(start.x, start.y, start.z),
            VertexCoord(end.x,   start.y, start.z),
            VertexCoord(end.z,   start.y, end.z),
            VertexCoord(start.z, start.y, end.z),
            !insideOut
    );

    //left
    this->AddQuad(
            VertexCoord(end.x, start.y, start.z),
            VertexCoord(end.x, end.y,   start.z),
            VertexCoord(end.x, end.y,   end.z),
            VertexCoord(end.x, start.y, end.z),
            !insideOut
            );

    //right
    this->AddQuad(
            VertexCoord(start.x, start.y, start.z),
            VertexCoord(start.x, end.y,   start.z),
            VertexCoord(start.x, end.y,   end.z),
            VertexCoord(start.x, start.y, end.z),
            insideOut
    );
}

VertexCoord::VertexCoord(float pX, float pY, float pZ) {
    this->x = pX;
    this->y = pY;
    this->z = pZ;
}

std::string VertexCoord::to_string() const {
    return "Vertex{x=" + std::to_string(x) + ",y=" + std::to_string(y) + ",z=" + std::to_string(z) + "}";
}
