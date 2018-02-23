#pragma once
#include <string>
#include "ServerConfig.hh"
#include "SynchronizedFile.hh"
/**
 * Configuration options for HTTPServer
 *
 * XXX: Provide a helper that can convert thrift/json to this config
 *      directly. We keep this object type-safe.
 */
class HTTPServerOptions {
 public:
// HTTPServerOptions(int listen_port, int threads, std::string ip, std::string root_dir);
  HTTPServerOptions(ServerConfig& server_tab);
  /**
   * Number of threads to start to handle requests. Note that this excludes
   * the thread you call `HTTPServer.start()` in.
   *
   * XXX: Put some perf numbers to help user decide how many threads to
   *      create.
   * XXX: Maybe support not creating any more worker threads and doing all
   *      the work in same thread when `threads == 0`.
   */
  ServerConfig& get_server_tab();
  
 private:
 // HTTPServerOptions();
  ServerConfig server_tab_;
  


  /**
   * This idle timeout serves two purposes -
   *
   * 1. How long to keep idle connections around before throwing them away.
   *
   * 2. If it takes client more than this time to send request, we fail the
   *    request.
   *
   * XXX: Maybe have separate time limit for both?
   */
  //std::chrono::milliseconds idleTimeout_;
  //bool enableContentCompression_;
  //uint64_t contentCompressionMinimumSize_;
  
};
