#include <stdio.h>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <map>
#include <unordered_map>
#include <../toml/toml.hpp>
#include "HTTPServerOptions.hh"
#include "ThreadPool.hh"
#include "ServersHandler.hh"
#include "ConfigOptions.hh"
#include "ServerConfig.hh"

int main(int argc, char* argv[])
{
  bool dry_run = false;

  // Check arguments
  if (argc < 2 || argc > 3)
  {
    //std::cerr << "usage: ./myhttpd [--dry-run] server.conf" << std::endl;
    return 1;
  }

  const char* arg;

  if (std::string(argv[1]) == "--dry-run")
    dry_run = true;

  if (argc < 3)
    arg = argv[1];
  else
    arg = argv[2];

  std::ifstream file(arg);
  std::string log_file;
  int nbserv = 0;

  try{

    // Parse the file
    toml::Data data = toml::parse(file);

    // Get first element: log_file
    
    try{
      log_file = toml::get<toml::String>(data.at("log_file"));
    }catch(...){
    }
  
    // Create array of server to delimit each server config
    std::vector<toml::Table> server = toml::get<toml::Array<toml::Table> >(data.at("server"));

    for (auto it = server.cbegin(); it != server.cend(); ++it)
      nbserv++;

    // ServerConfig *server_array[nbserv]; 
    std::vector<ServerConfig> server_array(nbserv);

    for (int i = 0; i < nbserv ; i++)
    {
      if (server.at(i).count("server_name"))
        server_array[i].set_server_name(toml::get<toml::String>(
              server.at(i).at("server_name")));
    
      if (server.at(i).count("port"))
        server_array[i].set_port(toml::get<toml::String>(
            server.at(i).at("port")));

      if (server.at(i).count("ip"))
        server_array[i].set_ip(toml::get<toml::String>(
            server.at(i).at("ip")));

      if (server.at(i).count("root_dir"))
        server_array[i].set_root_dir(toml::get<toml::String>(
            server.at(i).at("root_dir")));

      if (server.at(i).count("gzip"))
        server_array[i].set_gzip(toml::get<bool>(
            server.at(i).at("gzip")));

      if (server.at(i).count("log"))
        server_array[i].set_log(toml::get<bool>(
            server.at(i).at("log")));

      if (server.at(i).count("cgi_ext"))
        server_array[i].set_cgi_ext(toml::get<toml::String>(
            server.at(i).at("cgi_ext")));

      if (server.at(i).count("ssl_certificate"))
        server_array[i].set_ssl_certificate(toml::get<toml::String>(
            server.at(i).at("ssl_certificate")));

      if (server.at(i).count("ssl_certificate_key"))
        server_array[i].set_ssl_certificate_key(toml::get<toml::String>(
            server.at(i).at("ssl_certificate_key")));

      if (server.at(i).count("basic_auth"))
        server_array[i].set_basic_auth(toml::get<bool>(
            server.at(i).at("basic_auth")));

      if (server.at(i).count("basic_auth_file"))
        server_array[i].set_basic_auth_file(toml::get<toml::String>(
            server.at(i).at("basic_auth_file")));

      if (server.at(i).count("error"))
      {
       
          std::vector<std::vector<std::string>> temp = toml::get<std::vector<std::vector<std::string>>>(server.at(i).at("error"));

        std::map<std::string, std::string> custom_errors;
        for (unsigned int i = 0; i < temp.size(); ++i)
        {
          custom_errors[temp[i][0]] =  temp[i][1];
        }

        server_array[i].set_custom_error(custom_errors);
      }
    }
    
    //Checking Parsing Errors
    for (int i = 0; i < nbserv; i++)
    {
      if (!server_array[i].get_server_name().get())
      {
        //std::cerr << "Missing server_name." << std::endl;
        return 2;
      }
      if (!server_array[i].get_port().get())
      {
        //std::cerr << "Missing port." << std::endl;
        return 2;
      }
      if (!server_array[i].get_ip().get())
      {
        //std::cerr << "Missing ip." << std::endl;
        return 2;
      }
      if (!server_array[i].get_root_dir().get())
      {
        //std::cerr << "Missing root_dir." << std::endl;
        return 2;
      }
    }
   
     
    std::unordered_map<int, std::unordered_map<std::string, HTTPServerOptions>> servers_options;
    for (int i = 0; i < nbserv; i++)
    {
      //HTTPServerOptions options(atoi(server_array[i].get_port().getparam().c_str()), 8, server_array[i].get_ip().getparam());
      servers_options[atoi(server_array[i].get_port().getparam().c_str())].emplace(server_array[i].get_server_name().getparam(), server_array[i]);
    }

    //if dry_run set to true, just check and return. Launch server otherwise.
    if (!dry_run)
    {
      ServersHandler servers_handler(servers_options, log_file);
      return 0;
    }
    else
      return 0;

  }catch(...){
    //std::cerr << "Parsing error." <<std::endl;
    return 2;
  }
}
