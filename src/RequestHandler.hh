#pragma once
#include <ThreadPool.hh>
#include <ResponseBuilder.hh>


class RequestHandler {
 public:

  RequestHandler();
  //virtual ~RequestHandler();
  //void onRequest();

  /**
   * Invoked when request processing has been completed and nothing more
   * needs to be done. This may be a good place to log some stats and
   * clean up resources.
   */
  //virtual void requestComplete();

  /**
   * Submit a job to the default thread pool.
   */
  template <typename Func, typename... Args>
  inline auto submitJob(Func&& func, Args&&... args);

  private:
  //ResponseBuilder reponse_builder_;
};
