#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>
#include <thread>

template <typename T>
class ThreadSafeQueue
{
public:
    // Destructor.
    ~ThreadSafeQueue(void);
    /*
    Attempt to get the first value in the queue.
    Returns true if a value was successfully written to the out parameter, false otherwise.
     */
    bool tryPop(T& out);
    /*
    Get the first value in the queue.
    Will block until a value is available unless clear is called or the instance is destructed.
    Returns true if a value was successfully written to the out parameter, false otherwise.
     */
    bool waitPop(T& out);
    /*
    Push a new value onto the queue.
     */
    void push(T value);
    /*
    Check whether or not the queue is empty.
     */
    bool empty(void) const
    {
        std::lock_guard<std::mutex> lock{m_mutex};
        return m_queue.empty();
    }
    /*
    Clear all items from the queue.
     */
    void clear(void);
    /*
    Invalidate the queue.
    Used to ensure no conditions are being waited on in waitPop when
    a thread or the application is trying to exit.
    The queue is invalid after calling this method and it is an error
    to continue using a queue after this method has been called.
     */
    void invalidate(void);
    /*
    Returns whether or not this queue is valid.
    */
    bool isValid(void) const;

private:
    std::atomic_bool m_valid{true};
    mutable std::mutex m_mutex;
    std::queue<T> m_queue;
    std::condition_variable m_condition;
};

#include "ThreadSafeQueue.hxx"