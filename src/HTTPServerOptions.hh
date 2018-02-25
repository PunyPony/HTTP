#pragma once
#include <string>
#include "ServerConfig.hh"
/*
Configuration options for HTTPServer
*/
class HTTPServerOptions {
 public:
  HTTPServerOptions(ServerConfig& server_tab);
  ServerConfig& get_server_tab();
  
 private:
  ServerConfig server_tab_;
};
