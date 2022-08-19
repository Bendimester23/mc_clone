#ifndef MC_CLONE_2_CHUNK_H
#define MC_CLONE_2_CHUNK_H

#include <mutex>
#include <glad/glad.h>
#include <array>
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


    public:

        GLuint VAO;
        GLuint VBO;
        GLuint indexVBO;

        //Chunk();
        
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

        void Lock();

        void Unlock();
    };
} // namespace world

#endif // MC_CLONE_2_CHUNK_H