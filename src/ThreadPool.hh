#pragma once
#include "Renderer.hh"

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    void Destroy();
    void Initialize(size_t numThreads = std::thread::hardware_concurrency());
    void SubmitChunkAndRender(Renderer *renderer, FastNoiseLite noise, int chunkX, int chunkZ);

private:
    std::vector<std::thread> m_ThreadWorkers;
    std::queue<std::function<void()>> m_Tasks;

    mutable std::mutex m_QueueMutex;
    std::condition_variable m_ConditionVariable;

    bool m_Stop = true;
};

inline ThreadPool *GetThreadPool()
{
    static ThreadPool threadPool;
    return &threadPool;
}
