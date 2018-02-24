#include <vector>
#include <iostream>
#include <memory>
#include "HTTPServerOptions.hh"
#include "ServersHandler.hh"

namespace ugly
{
    volatile std::sig_atomic_t SignalStatus;
    ServersHandler* server_handler_;
}

void server_thread(HTTPServer&& server)
{
    int sock;
    if (!server.init(sock))
        server.start(sock);
    else
        std::cout << "ERROR launching server" << std::endl; //fixme: proper error
}

ServersHandler::ServersHandler(std::vector<HTTPServerOptions> options, std::string log_file_path)
: log_file_path_(log_file_path)
{
    ugly::server_handler_ = this;
    SetSignals();
    for (auto it : options)
    {
        servers_.emplace_back(it, log_file_path_);
        //check return codes
    }
    for (auto it : servers_)
    {
        th_servers_.emplace_back(server_thread, it);
    }
}
ServersHandler::~ServersHandler()
{
    for (auto& it : th_servers_)
    {
        it.join();
    }
}

std::vector<HTTPServer> ServersHandler::get_servers()
{
    return servers_;
}

void signal_handler(int signal)
{
    ugly::SignalStatus = signal;
    std::cout << "SIGINT : Signal value : " << ugly::SignalStatus << '\n';
    for (auto& it : ugly::server_handler_->get_servers())
        it.stop();
}

void signal_ignored(int signal)
{
    ugly::SignalStatus = signal;
    std::cout << "Signal value : " << ugly::SignalStatus << '\n';
}


void SetSignals()
{
    std::signal(SIGINT, signal_handler);
    std::signal(SIGABRT || SIGSTOP || SIGTSTP || SIGFPE || SIGILL || SIGTERM, signal_ignored);
}