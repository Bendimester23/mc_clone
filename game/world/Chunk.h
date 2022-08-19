#ifndef MC_CLONE_2_CHUNK_H
#define MC_CLONE_2_CHUNK_H

#include <mutex>
#include <glad/glad.h>

namespace world
{
    class Chunk
    {
        char m_Blocks[16][16][16];
        bool m_Dirty;
        bool m_NeedsUpload;
        std::mutex m_Mutex;


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