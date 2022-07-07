//
// Created by bendi on 2022. 07. 05..
//

#ifndef MC_CLONE_WORLD_H
#define MC_CLONE_WORLD_H


#include <vector>
#include "Chunk.h"
#include "../render/mesh/ChunkMesh.h"
#include "../shader/ShaderProgram.h"
#include "../render/Camera.h"
#include "../render/texture/Texture.h"

namespace world {
    class World {
        std::vector<chunk::Chunk> chunks;
        std::vector<chunk::ChunkMesh> meshes;
        ShaderProgram chunkShader;
        Texture atlasTexture;

        void LoadChunk(int x, int y, int z);

    public:
        World(Camera *c);

        Camera *cam;

        void Update();

        void Render();

        void Init();

        bool IsAirAt(glm::vec3 pos);
    };
}

#endif //MC_CLONE_WORLD_H
