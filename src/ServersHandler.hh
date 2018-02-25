#pragma once
#include <vector>
#include <csignal>
#include <thread>
#include "HTTPServer.hh"
#include "HTTPServerOptions.hh"

class ServersHandler
{
 public:
    ServersHandler(std::unordered_map<int, std::unordered_map<std::string, HTTPServerOptions>> options, std::string log_file_path);
    ~ServersHandler();
    std::vector<HTTPServer> get_servers();


 private:
    std::string log_file_path_;
    std::vector<HTTPServer> servers_;
    std::vector<std::thread> th_servers_;
};

void SetSignals();
void signal_handler(int signal);
void signal_ignored(int signal);

namespace ugly
{
  extern volatile std::sig_atomic_t SignalStatus;
  extern ServersHandler* server_handler_;
}

