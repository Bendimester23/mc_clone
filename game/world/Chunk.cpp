#include "./Chunk.h"

namespace world
{
    Chunk::Chunk(ChunkCoord pos) noexcept: m_Pos(pos), m_Uploaded(false), m_IndicesCount(0)
    {
        // Set everything to air
        // this->m_Mutex.lock();
        for (char x = 0; x < 16; x++)
        {
            for (char y = 0; y < 16; y++)
            {
                for (char z = 0; z < 16; z++)
                {
                    this->SetBlockAt(x, y, z, 0);
                }
            }
        }
        // this->m_Mutex.unlock();
    }

    char Chunk::GetBlockAt(char x, char y, char z)
    {
        // this->m_Mutex.lock();
        if (x < 0 || x > 15 || y < 0 || y > 15 || z < 0 || z > 15) return 0;
        return m_Blocks[x + (y * 16) + (z * 256)];
        // this->m_Mutex.unlock();
    }

    void Chunk::SetBlockAt(char x, char y, char z, char block)
    {
        // this->m_Mutex.lock();
        this->m_Blocks[x + (y * 16) + (z * 256)] = block;
        // this->m_Mutex.unlock();
    }

    void Chunk::MarkDirty()
    {
        // this->m_Mutex.lock();
        this->m_Dirty = true;
        this->m_NeedsUpload = false;
        // this->m_Mutex.unlock();
    }

    void Chunk::MarkDone()
    {
        // this->m_Mutex.lock();
        this->m_Dirty = false;
        this->m_NeedsUpload = true;
        // this->m_Mutex.unlock();
    }

    void Chunk::MarkClean()
    {
        // this->m_Mutex.lock();
        this->m_Dirty = false;
        this->m_NeedsUpload = false;
        // this->m_Mutex.unlock();
    }

    void Chunk::Lock()
    {
        // this->m_Mutex.lock();
    }

    void Chunk::Unlock()
    {
        // this->m_Mutex.unlock();
    }

    void Chunk::Upload()
    {
        if (this->m_Vertices.size() == 0 || this->m_Indices.size() == 0) return;
        if (!this->m_Uploaded)
        {
            glGenVertexArrays(1, &this->VAO);

            glGenBuffers(1, &this->VBO);
            glGenBuffers(1, &indexVBO);
            this->m_Uploaded = true;
        }
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, this->m_Vertices.size() * sizeof(unsigned int), this->m_Vertices.data(), GL_DYNAMIC_DRAW);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
        
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
            this->m_Indices.size() * 4, 
            this->m_Indices.data(), 
            GL_DYNAMIC_DRAW);
        this->m_IndicesCount = this->m_Indices.size();
    }

    ChunkCoord Chunk::GetPosition() {
        return this->m_Pos;
    }

    void Chunk::Render() {
        if (!this->m_Uploaded) return;
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);

        glDrawElements(
                    GL_TRIANGLES,
                    (int)m_IndicesCount,
                    GL_UNSIGNED_SHORT,
                    nullptr);

        glBindVertexArray(0);
    }
} // namespace world
