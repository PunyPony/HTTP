#include <cstddef>
#include <string>
#include <HTTPServerOptions.hh>

 HTTPServerOptions::HTTPServerOptions(Serverconfig server_array)
 : server_array_(server_array)
 {}

 Serverconfig& HTTPServerOptions::get_server_array()
{
  return server_array_;
}
