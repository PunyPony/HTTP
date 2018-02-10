#include <vector>
#include <iostream>
#include <HTTPServerOptions.hh>
#include <ServersHandler.hh>

namespace ugly
{
    volatile std::sig_atomic_t SignalStatus;
}

ServersHandler::ServersHandler(std::vector<HTTPServerOptions> options)
    : servers_()
{
    SetSignals();
    for (std::vector<HTTPServerOptions>::iterator it = options.begin(); 
    it != options.end(); ++it)
    {
        HTTPServer server(*it);
        servers_.push_back(&server);
        server.start();
    }
    
}

std::vector<HTTPServer*> ServersHandler::get_servers()
{
    return servers_;
}

void signal_handler(int signal)
{
    ugly::SignalStatus = signal;
    std::cout << "SIGINT : Signal value : " << ugly::SignalStatus << '\n';  
    for (std::vector<HTTPServer*>::iterator it = ugly::server_handler_->get_servers().begin(); 
    it != ugly::server_handler_->get_servers().end(); ++it)
        (*it)->stop();
}

void signal_ignored(int signal)
{
    ugly::SignalStatus = signal;
    std::cout << "Signal value : " << ugly::SignalStatus << '\n'; 
}


void SetSignals()
{
    std::signal(SIGINT, signal_handler);
    std::signal(SIGABRT || SIGFPE || SIGILL || SIGTERM, signal_ignored);
}