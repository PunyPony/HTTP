#include <cstddef>
#include <string>
#include <HTTPServerOptions.hh>

 HTTPServerOptions::HTTPServerOptions(int listen_port, int threads, std::string ip)
 : listen_port_(listen_port), threads_(threads), ip_(ip)
 {}