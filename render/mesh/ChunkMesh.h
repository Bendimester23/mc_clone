//
// Created by bendi on 2022. 07. 05..
//

#ifndef MC_CLONE_CHUNKMESH_H
#define MC_CLONE_CHUNKMESH_H


#include "../../world/Chunk.h"
#include "GL/glew.h"
#include <string>
#include <cstring>
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include "ChunkMeshBuilder.h"

namespace chunk {
    class ChunkMesh {
        GLuint m_VAO{}, m_VBO{}, m_IndexBuffer{};
        bool m_Bound = false;
        int indicesCount = 0;

        void Rebuild();

    public:
        Chunk *m_Chunk;

        explicit ChunkMesh(Chunk *chunk);

        void Bind();

        void UnBind();

        void Render();

        bool Update();

        bool IsAirAt(BlockCoordinate pos);
    };
}

#endif //MC_CLONE_CHUNKMESH_H
