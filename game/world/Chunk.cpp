#include "./Chunk.h"

namespace world
{
    Chunk::Chunk() {
        //Set everything to air
        for (char x = 0; x < 16; x++)
        {
            for (char y = 0; y < 16; y++)
            {
                for (char z = 0; z < 16; z++)
                {
                    this->m_Blocks[x][y][z] = 0;
                }
            }
        }
    }

    char Chunk::GetBlockAt(char x, char y, char z) {
        this->m_Mutex.lock();
        return m_Blocks[x][y][z];
        this->m_Mutex.unlock();
    }

    void Chunk::SetBlockAt(char x, char y, char z, char block) {
        this->m_Mutex.lock();
        this->m_Blocks[x][y][z] = block;
        this->m_Mutex.unlock();
    }

    void Chunk::MarkDirty() {
        this->m_Mutex.lock();
        this->m_Dirty = true;
        this->m_NeedsUpload = false;
        this->m_Mutex.unlock();
    }

    void Chunk::MarkDone() {
        this->m_Mutex.lock();
        this->m_Dirty = false;
        this->m_NeedsUpload = true;
        this->m_Mutex.unlock();
    }

    void Chunk::MarkClean() {
        this->m_Mutex.lock();
        this->m_Dirty = false;
        this->m_NeedsUpload = false;
        this->m_Mutex.unlock();
    }

    void Chunk::Lock() {
        this->m_Mutex.lock();
    }

    void Chunk::Unlock() {
        this->m_Mutex.unlock();
    }
} // namespace world
