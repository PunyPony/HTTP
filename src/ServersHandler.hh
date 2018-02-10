#pragma once
#include <vector>
#include <csignal>
#include <HTTPServer.hh>
#include <HTTPServerOptions.hh>

class ServersHandler
{
 public:
    ServersHandler(std::vector<HTTPServerOptions> options);
    std::vector<HTTPServer*> get_servers();


 private:
    std::vector<HTTPServer*> servers_;
};

void SetSignals();
void signal_handler(int signal);
void signal_ignored(int signal);

namespace ugly
{
  extern volatile std::sig_atomic_t SignalStatus;
  extern ServersHandler* server_handler_;
}

