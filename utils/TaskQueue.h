#ifndef MC_CLONE_2_TASKQUEUE_H
#define MC_CLONE_2_TASKQUEUE_H

#include <list>
#include <mutex>
#include <chrono>
#include <thread>
#include <array>

/**
 * @brief basic utilities
 * These might exist in the stl, but i was too lasy to search
 */
namespace utils
{
    /**
     * @brief a minimal, thread safe task queue implementation
     *
     */
    template <typename T>
    class TaskQueue
    {
        std::list<T> m_Store;
        std::mutex m_Mutex;

    public:
        /**
         * @brief adds a task to the queue
         *
         * @param task
         */
        void push_back(T task)
        {
            this->m_Mutex.lock();
            this->m_Store.push_back(task);
            this->m_Mutex.unlock();
        }

        /**
         * @brief fetches a task from the queue, blocks if no task is present
         *
         * @return T the fetched task
         */
        T fetch(bool* running)
        {
            while (*running)
            {
                this->m_Mutex.lock();
                if (!this->m_Store.empty())
                {
                    auto task = this->m_Store.front();
                    this->m_Store.pop_front();
                    this->m_Mutex.unlock();
                    return task;
                }
                this->m_Mutex.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            return T();
        }

        /**
         * @brief Tries to fetch every element from the queue, but doesn't blocks if it's empty
         * 
         * @return std::array<T> every element of the queue
         */
        std::vector<T> try_fetch_all() {
            this->m_Mutex.lock();
            if (this->m_Store.empty()) {
                this->m_Mutex.unlock();
                return std::vector<T>();
            }
            
            std::vector<T> res;

            res.reserve(this->m_Store.size());

            size_t i = 0;
            for (auto& e : this->m_Store) {
                res[i++] = e;
            }
            this->m_Store.clear();
            this->m_Mutex.unlock();
            return res;
        }

        void clear() {
            this->m_Mutex.lock();
            this->m_Store.clear();
            this->m_Mutex.unlock();
        }
    };
} // namespace utils

#endif // MC_CLONE_2_TASKQUEUE_H