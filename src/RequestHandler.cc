#include <RequestHandler.hh>
#include <ThreadPool.hh>

RequestHandler::RequestHandler(int threads)
  : thread_pool_(threads)
{}
