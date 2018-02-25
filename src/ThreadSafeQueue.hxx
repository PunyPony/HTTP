#include "ThreadSafeQueue.hh"

template <typename T>
ThreadSafeQueue<T>::~ThreadSafeQueue(void)
{
    invalidate();
}

template <typename T>
bool ThreadSafeQueue<T>::tryPop(T& out)
{
    std::lock_guard<std::mutex> lock{m_mutex};
    if(m_queue.empty() || !m_valid)
    {
        return false;
    }
    out = std::move(m_queue.front());
    m_queue.pop();
    return true;
}

template <typename T>
bool ThreadSafeQueue<T>::waitPop(T& out)
{
    std::unique_lock<std::mutex> lock{m_mutex};
    m_condition.wait(lock, [this]()
    {
        return !m_queue.empty() || !m_valid;
    });
    /*
    Using the condition in the predicate ensures that spurious wakeups with a valid
    but empty queue will not proceed, so only need to check for validity before proceeding.
    */
    if(!m_valid)
    {
        return false;
    }
    out = std::move(m_queue.front());
    m_queue.pop();
    return true;
}

template <typename T>
void ThreadSafeQueue<T>::push(T value)
{
    std::lock_guard<std::mutex> lock{m_mutex};
    m_queue.push(std::move(value));
    m_condition.notify_one();
}

template <typename T>
void ThreadSafeQueue<T>::clear(void)
{
    std::lock_guard<std::mutex> lock{m_mutex};
    while(!m_queue.empty())
    {
        m_queue.pop();
    }
    m_condition.notify_all();
}

template <typename T>
void ThreadSafeQueue<T>::invalidate(void)
{
    std::lock_guard<std::mutex> lock{m_mutex};
    m_valid = false;
    m_condition.notify_all();
}

template <typename T>
bool ThreadSafeQueue<T>::isValid(void) const
{
    std::lock_guard<std::mutex> lock{m_mutex};
    return m_valid;
}