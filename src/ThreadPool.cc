#include "ThreadPool.hh"

#include "Utils/Logger.hh"

void ThreadPool::Initialize(size_t numThreads)
{
    LogInfo("Starting with thread pool of {} threads", numThreads);

    m_Stop = false;

    for (size_t i = 0; i < numThreads; ++i)
    {
        m_ThreadWorkers.emplace_back([this] {
            while (true)
            {
                std::function<void()> task;

                {
                    std::unique_lock lock(m_QueueMutex);
                    m_ConditionVariable.wait(lock, [this] {
                        return m_Stop || !m_Tasks.empty();
                    });

                    if (m_Stop && m_Tasks.empty())
                    {
                        return;
                    }

                    task = std::move(m_Tasks.front());
                    m_Tasks.pop();
                }

                task();
            }
        });
    }
}

void ThreadPool::Destroy()
{
    {
        std::unique_lock lock(m_QueueMutex);
        m_Stop = true;
    }

    m_ConditionVariable.notify_all();

    for (auto &worker : m_ThreadWorkers)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
}

void ThreadPool::SubmitChunkAndRender(Renderer *renderer, FastNoiseLite noise, int chunkX, int chunkZ)
{
    {
        std::unique_lock lock(m_QueueMutex);

        if (m_Stop)
        {
            throw std::runtime_error("Cannot enqueue on stopped ThreadPool");
        }

        m_Tasks.emplace([renderer, noise, chunkX, chunkZ]() {
            Chunk chunk;
            chunk.Initialize(noise, glm::ivec2(chunkX, chunkZ));

            LogDebug("Adding chunk: ({},{})", chunkX, chunkZ);

            renderer->RenderChunk(noise, std::move(chunk));
        });
    }

    m_ConditionVariable.notify_one();
}
