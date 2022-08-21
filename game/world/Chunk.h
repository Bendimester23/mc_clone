#ifndef MC_CLONE_2_CHUNK_H
#define MC_CLONE_2_CHUNK_H

#include <mutex>
#include <glad/glad.h>
#include <array>
#include <spdlog/spdlog.h>
#include <vector>
#include "../../config.h"
#include "./ChunkCoord.h"

namespace world
{
    class Chunk
    {
        ChunkCoord m_Pos;
        std::array<char, CHUNK_VOLUME> m_Blocks;
        bool m_Dirty;
        bool m_NeedsUpload;
        //std::mutex m_Mutex;
        bool m_Uploaded;
        size_t m_IndicesCount;

    public:

        GLuint VAO;
        GLuint VBO;
        GLuint indexVBO;

        std::vector<unsigned int> m_Vertices;
        std::vector<unsigned short> m_Indices;

        Chunk(ChunkCoord pos) noexcept;
        
        void Init();

        char GetBlockAt(char x, char y, char z);

        void SetBlockAt(char x, char y, char z, char block);

        void MarkDirty();

        void MarkDone();

        ChunkCoord GetPosition();

        /**
         * @brief marks the chunk both not dirty and uploaded
         * 
         */
        void MarkClean();

        void Upload();

        void Lock();

        void Unlock();

        void Render();
    };
} // namespace world

#endif // MC_CLONE_2_CHUNK_H