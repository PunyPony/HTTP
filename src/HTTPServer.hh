#pragma once
#include <memory>
#include <unordered_map>

#include "HTTPServerOptions.hh"
#include "SynchronizedFile.hh"
#include "Cache.hh"



class HTTPServer {
 public:
  /*
  Create a new HTTPServer
   */
  HTTPServer(std::unordered_map<std::string, HTTPServerOptions> options, std::string log_file_path_);
  ~HTTPServer();
  /*
  Start HTTPServer.
  */
  int init(int& sock);
  int start(int sock);
  /*
  Stop listening on bound ports. (Stop accepting new work).
  It does not wait for pending work to complete.
  You must still invoke stop() before destroying the server.
  You do NOT need to invoke this before calling stop().
  This can be called from any thread, and it is idempotent.
  However, it may only be called **after** start() has called onSuccess.
   */
  void stopListening();
  /*
  Stop HTTPServer.
  Can be called from any thread, but only after start() has called
  onSuccess.  Server will stop listening for new connections and will
  wait for running requests to finish.
  TODO: Separate method to do hard shutdown ?
   */
  void stop();
  int getListenSocket() const;
  HTTPServerOptions* get_server_options(std::string server_name);
  std::shared_ptr<SynchronizedFile>& get_log_file();
 private:
     std::unordered_map<std::string, HTTPServerOptions> options_;
  std::shared_ptr<SynchronizedFile> log_file_;
};
