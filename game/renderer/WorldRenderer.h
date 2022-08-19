//
// Created by bendi on 2022.08.15..
//

#ifndef MC_CLONE_2_WORLDRENDERER_H
#define MC_CLONE_2_WORLDRENDERER_H

#include <glad/glad.h>
#include "../gl/Texture.h"
#include "../gl/ShaderProgram.h"
#include "../mesh/ChunkMeshBuilder.h"
#include <glm/glm.hpp>
#include "../world/Chunk.h"
#include "../world/World.h"

namespace renderer {

    class WorldRenderer {
        GLuint VAO{};
        GLuint VBO{};
        GLuint indexVBO{};
        gl::Texture m_AtlasTexture;
        gl::ShaderProgram m_ChunkShader;
        size_t m_IndicesCount;
        mesh::ChunkMeshBuilder b;
        world::World m_World;

    public:
        WorldRenderer();

        void Render(double delta, glm::mat4 matrix, bool wireframe);

        void Update(double delta);

        void AddBlock(char x, char y, char z);

        world::World* GetWorld();
    };

} // renderer

#endif //MC_CLONE_2_WORLDRENDERER_H
