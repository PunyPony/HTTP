#include "ThreadPool.hh"

ThreadPool::ThreadPool(const std::uint32_t numThreads)
    : m_done{ false }, m_workQueue{}, m_threads{}
{
    try
    {
        for (std::uint32_t i = 0u; i < numThreads; ++i)
            m_threads.emplace_back(&ThreadPool::worker, this);
    }
    catch (...)
    {
        destroy();
        throw;
    }
}

 ThreadPool::ThreadPool(void)
    : ThreadPool{ std::max(std::thread::hardware_concurrency(), 2u) - 1u }
{
    /*
        * Always create at least one thread.  If hardware_concurrency() returns 0,
        * subtracting one would turn it to UINT_MAX, so get the maximum of
        * hardware_concurrency() and 2 before subtracting 1.
        */
}


ThreadPool::~ThreadPool(void)
{
    destroy();
}

void ThreadPool::destroy(void)
{
    m_done = true;
    m_workQueue.invalidate();
    for (auto& thread : m_threads)
        if (thread.joinable())
        thread.join();
}

void ThreadPool::worker(void)
{
    while (!m_done)
    {
        std::unique_ptr<IThreadTask> pTask{ nullptr };
        if (m_workQueue.waitPop(pTask))
            pTask->execute();
    }
}