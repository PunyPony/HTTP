#pragma once
#include <algorithm>
#include <atomic>
#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
#include "ThreadSafeQueue.hh"

class ThreadPool
{ 
public:
    /*
    A wrapper around a std::future that adds the behavior of futures returned from std::async.
    Specifically, this object will block and wait for execution to finish before going out of scope.
     */
    template <typename T>
    class TaskFuture
    {
    public:
        TaskFuture(std::future<T>&& future);
        TaskFuture(const TaskFuture& rhs) = delete;
        TaskFuture& operator=(const TaskFuture& rhs) = delete;
        TaskFuture(TaskFuture&& other) = default;
        TaskFuture& operator=(TaskFuture&& other) = default;
        ~TaskFuture(void);
        auto get(void);
    private:
        std::future<T> m_future;
    };

    //Constructor.
    ThreadPool(void);
    //Constructor.
    explicit ThreadPool(const std::uint32_t numThreads);
    //Non-copyable.
    ThreadPool(const ThreadPool& rhs) = delete;
    //Non-assignable.
    ThreadPool& operator=(const ThreadPool& rhs) = delete;
    //Destructor.
    ~ThreadPool(void);
    //Submit a job to be run by the thread pool.
    template <typename Func, typename... Args>
    auto submit(Func&& func, Args&&... args);

private:
     class IThreadTask
    {
    public:
        IThreadTask(void) = default;
        virtual ~IThreadTask(void) = default;
        IThreadTask(const IThreadTask& rhs) = delete;
        IThreadTask& operator=(const IThreadTask& rhs) = delete;
        IThreadTask(IThreadTask&& other) = default;
        IThreadTask& operator=(IThreadTask&& other) = default;
        // Run the task.
        virtual void execute() = 0;
    };

    template <typename Func>
    class ThreadTask : public IThreadTask
    {
    public:
        ThreadTask(Func&& func);
        ~ThreadTask(void) override = default;
        ThreadTask(const ThreadTask& rhs) = delete;
        ThreadTask& operator=(const ThreadTask& rhs) = delete;
        ThreadTask(ThreadTask&& other) = default;
        ThreadTask& operator=(ThreadTask&& other) = default;
        //Run the task.
        void execute() override;
    private:
        Func m_func;
    };
    //Constantly running function each thread uses to acquire work items from the queue.
    void worker(void);
    //Invalidates the queue and joins all running threads.
    void destroy(void);
    std::atomic_bool m_done;
    ThreadSafeQueue<std::unique_ptr<IThreadTask>> m_workQueue;
    std::vector<std::thread> m_threads;
};

namespace DefaultThreadPool
{
    /*
    - Get the default thread pool for the application.
    - This pool is created with std::thread::hardware_concurrency() - 1 threads.
    */
    inline ThreadPool& getThreadPool(void)
    {
        static ThreadPool defaultPool;
        return defaultPool;
    }
    //Submit a job to the default thread pool.
    template <typename Func, typename... Args>
    inline auto submitJob(Func&& func, Args&&... args)
    {
        return getThreadPool().submit(std::forward<Func>(func), std::forward<Args>(args)...);
    }
}

#include "ThreadPool.hxx"