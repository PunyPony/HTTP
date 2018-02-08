#include <HTTPServer.hh>
#include <HTTPServerOptions.hh>

/**
   * Verify options
*/
class AcceptorFactory {
 public:
 private:
};

HTTPServer::HTTPServer(HTTPServerOptions options)
  {
  // Insert a filter to fail all the CONNECT request, if required
  // Add Content Compression filter (gzip), if needed. Should be
  // final filter
  }

HTTPServer::~HTTPServer() {}

class HandlerCallbacks {
 public: 
  /*
  void threadStarted(ThreadPoolExecutor::ThreadHandle* h) override 
  {}
  void threadStopped(ThreadPoolExecutor::ThreadHandle* h) override 
  {}
  */
 private:
};


void HTTPServer::start() 
{}

void HTTPServer::stopListening() 
{}

void HTTPServer::stop() 
{
  stopListening();
}
