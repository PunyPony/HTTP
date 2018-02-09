#pragma once
#include <ThreadPool.hh>


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
  private:
  ThreadPool thread_pool_;


};
