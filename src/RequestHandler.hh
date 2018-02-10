#pragma once
#include <ThreadPool.hh>
#include <ResponseBuilder.hh>


class RequestHandler {
 public:

  RequestHandler(int threads);
  //virtual ~RequestHandler();
  //void onRequest();

  /**
   * Invoked when request processing has been completed and nothing more
   * needs to be done. This may be a good place to log some stats and
   * clean up resources.
   */
  //virtual void requestComplete();
  /**
   * Get the default thread pool for the application.
   * This pool is created with std::thread::hardware_concurrency() - 1 threads.
   */
  
  ThreadPool& getThreadPool();

  /**
   * Submit a job to the default thread pool.
   */
  template <typename Func, typename... Args>
  inline auto submitJob(Func&& func, Args&&... args);

  private:
  ThreadPool thread_pool_;
  ResponseBuilder reponse_builder_;
};
