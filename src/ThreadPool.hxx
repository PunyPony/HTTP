#include <ThreadPool.hh>

template <typename Func, typename... Args>
auto ThreadPool::submit(Func&& func, Args&&... args)
{
    auto boundTask = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    using ResultType = std::result_of_t<decltype(boundTask)()>;
    using PackagedTask = std::packaged_task<ResultType()>;
    using TaskType = ThreadTask<PackagedTask>;

    PackagedTask task{ std::move(boundTask) };
    TaskFuture<ResultType> result{ task.get_future() };
    m_workQueue.push(std::make_unique<TaskType>(std::move(task)));
    return result;
}

template <typename T>
ThreadPool::TaskFuture<T>::TaskFuture(std::future<T>&& future)
            :m_future{ std::move(future) }
{}

template <typename T>
ThreadPool::TaskFuture<T>::~TaskFuture(void)
{
    if (m_future.valid())
    {
        //m_future.get();
    }
}

template <typename T>
auto ThreadPool::TaskFuture<T>::get(void)
{
    return m_future.get();
}

template <typename Func>
ThreadPool::ThreadTask<Func>::ThreadTask(Func&& func)
    :m_func{ std::move(func) }
{}

template <typename Func>
void ThreadPool::ThreadTask<Func>::execute()
{
    m_func();
}
