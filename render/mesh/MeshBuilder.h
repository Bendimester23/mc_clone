//
// Created by bendi on 2022. 06. 29..
//

#ifndef MC_CLONE_MESHBUILDER_H
#define MC_CLONE_MESHBUILDER_H

#include <vector>
#include <string>
#include <set>

struct VertexCoord {
    VertexCoord(float pX, float pY, float pZ);

    float x{}, y{}, z{};

    std::string to_string() const;
};

class MeshBuilder {
    std::vector<VertexCoord> m_Vertices;
    std::vector<unsigned short> m_Indices;

public:
    MeshBuilder();

    void AddVertex(VertexCoord vert);

    void AddQuad(VertexCoord a, VertexCoord b, VertexCoord c, VertexCoord d, bool cc);

    void AddCube(VertexCoord start, VertexCoord end, bool insideOut);

    std::vector<float> GetVertices();

    std::vector<unsigned short> GetIndices();

    int GetIndicesCount();
};


#endif //MC_CLONE_MESHBUILDER_H
