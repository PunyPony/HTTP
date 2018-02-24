#include <cstddef>
#include <string>
#include "HTTPServerOptions.hh"

HTTPServerOptions::HTTPServerOptions(ServerConfig& server_tab)
    : server_tab_(server_tab)
{}

ServerConfig& HTTPServerOptions::get_server_tab()
{
    return server_tab_;
}
