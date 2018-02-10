#include <RequestHandler.hh>
//#include <ThreadPool.hh>

template <typename Func, typename... Args>
inline auto submitJob(Func&& func, Args&&... args)
{
    return getThreadPool().submit(std::forward<Func>(func), std::forward<Args>(args)...);
}
