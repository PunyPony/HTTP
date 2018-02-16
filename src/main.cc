#include <stdio.h>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <../toml/toml.hpp>
#include <HTTPServerOptions.hh>
#include <ThreadPool.hh>
#include <ServersHandler.hh>
#include <ConfigOptions.hh>
#include <ServerConfig.hh>

int main(int argc, char* argv[])
{
  //Check arguments
  if (argc != 2)
  {
    std::cerr << "usage: ./myhttpd server.conf" << std::endl;
    return 1;
  }

  const char* arg = argv[1];
  std::ifstream file(arg);

  if (not file.good())
    return 1;

  //Parse the file
  toml::Data data = toml::parse(file);
  /*
  if (data == None)
    return 2; il faut trouver la valeur de data quand ça foire*/

  //FIXME CHECK VALUES - IF NONE THEN RETURN 2

  //Get 2 first elements: log_file and dry_run
//  std::string log_file = toml::get<toml::String>(data.at("log_file"));
  bool dry_run = toml::get<bool>(data.at("dry_run"));
  
  
    //Create array of server to delimit each server config
    std::vector<toml::Table> server = toml::get<toml::Array<toml::Table> >(data.at("server"));

    int nbserv = 0;
    for (auto it = server.cbegin(); it != server.cend(); ++it)
      nbserv++;

   // ServerConfig *server_array[nbserv];
   
    std::vector<ServerConfig*> server_array(nbserv);

    for (int i = 0; i < nbserv; i++)
      server_array[i] = new ServerConfig;

    for (int i = 0; i < nbserv ; i++)
    {
      if (server.at(i).count("server_name"))
        server_array[i]->set_server_name(toml::get<toml::String>(
              server.at(i).at("server_name")));

      if (server.at(i).count("port"))
        server_array[i]->set_port(toml::get<toml::String>(
            server.at(i).at("port")));

      if (server.at(i).count("ip"))
        server_array[i]->set_ip(toml::get<toml::String>(
            server.at(i).at("ip")));

      if (server.at(i).count("root_dir"))
        server_array[i]->set_root_dir(toml::get<toml::String>(
            server.at(i).at("root_dir")));

      if (server.at(i).count("gzip"))
        server_array[i]->set_gzip(toml::get<bool>(
            server.at(i).at("gzip")));

      if (server.at(i).count("log"))
        server_array[i]->set_log(toml::get<bool>(
            server.at(i).at("log")));

      if (server.at(i).count("cgi_ext"))
        server_array[i]->set_cgi_ext(toml::get<toml::String>(
            server.at(i).at("cgi_ext")));

      if (server.at(i).count("ssl_certificate"))
        server_array[i]->set_ssl_certificate(toml::get<toml::String>(
            server.at(i).at("ssl_certificate")));

      if (server.at(i).count("ssl_certificate_key"))
        server_array[i]->set_ssl_certificate_key(toml::get<toml::String>(
            server.at(i).at("ssl_certificate_key")));

      if (server.at(i).count("basic_auth"))
        server_array[i]->set_basic_auth(toml::get<bool>(
            server.at(i).at("basic_auth")));

      if (server.at(i).count("basic_auth_file"))
        server_array[i]->set_basic_auth_file(toml::get<toml::String>(
            server.at(i).at("basic_auth_file")));
    }

    //Checking Parsing Errors
    for (int i = 0; i < nbserv; i++)
    {
      if (!server_array[i]->get_server_name().get())
        return 2;
      if (!server_array[i]->get_port().get())
        return 2;
      if (!server_array[i]->get_ip().get())
        return 2;
      if (!server_array[i]->get_root_dir().get())
        return 2;
    }
  
  
/* 
  for (int i = 0; i < nbserv; i++)
  {
    std::cout << server_array[i]->get_port().getparam() << std::endl;
    std::cout << server_array[i]->get_ip().getparam() << std::endl;
    if (server_array[i]->get_cgi_ext().get())
      std::cout << "Param: true" << std::endl;
    else
      std::cout << "Param: false" << std::endl;
  }
*/

    //if dry_run set to true, just check and return. Launch server otherwise.
    if (!dry_run)
    {
      std::vector<HTTPServerOptions> servers_options;
      for (int i = 0; i < nbserv; i++)
      {
        HTTPServerOptions options(atoi(server_array[i]->get_port().getparam().c_str()), 8, server_array[i]->get_ip().getparam());
        servers_options.push_back(options);
      }

      //Check log_file
      std::string log_file;
          try {
               log_file = toml::get<toml::String>(data.at("log_file"));
               std::cout << "Log file activated" << std::endl;
              }
          catch (...){
              std::cout << "No log file required." <<std::endl;
          }

          try {
            if (data.count("log_file"))
              std::cout << "Launch server with log_file" << std::endl;
          }
          catch(...){
            std::cout << "Launch server without log_file" << std::endl;
          }

      ServersHandler servers_handler(servers_options);
      return 0;
    }
    else
      return 0;
}
