//
// Created by bendi on 2022.08.15..
//

#ifndef MC_CLONE_2_WORLDRENDERER_H
#define MC_CLONE_2_WORLDRENDERER_H

#include <cmath>
#include <glad/glad.h>
#include "../gl/Texture.h"
#include "../gl/ShaderProgram.h"
#include "../mesh/ChunkMeshBuilder.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include "../world/Chunk.h"
#include "../world/World.h"

namespace renderer {

    class WorldRenderer {
        gl::Texture m_AtlasTexture;
        gl::ShaderProgram m_ChunkShader;
        world::World m_World;
        glm::vec3 m_PrevPos;
        int maxDst = 5;
        int m_Frame;

    public:
        WorldRenderer();

        void Render(double delta, glm::mat4 matrix, bool wireframe);

        void Update(double delta, glm::vec3 pos);

        world::World* GetWorld();
    };

} // renderer

#endif //MC_CLONE_2_WORLDRENDERER_H
