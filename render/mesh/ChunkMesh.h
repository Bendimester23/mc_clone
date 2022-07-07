//
// Created by bendi on 2022. 07. 05..
//

#ifndef MC_CLONE_CHUNKMESH_H
#define MC_CLONE_CHUNKMESH_H


#include "../../world/Chunk.h"
#include "GL/glew.h"
#include <string>
#include "MeshBuilder.h"

class ChunkMesh {
    GLuint m_VAO{}, m_VBO{}, m_IndexBuffer{};
    bool m_Bound = false;
    int indicesCount = 0;

    void Rebuild();

    bool IsAirAt(BlockCoordinate pos);
public:
    Chunk *m_Chunk;
    explicit ChunkMesh(Chunk* chunk);

    void Bind();

    void UnBind();

    void Render();

    void Update();
};


#endif //MC_CLONE_CHUNKMESH_H
