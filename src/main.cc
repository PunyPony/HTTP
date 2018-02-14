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



int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    printf("Missing file in command line. \n");
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
  std::string log_file = toml::get<toml::String>(data.at("log_file"));
  bool dry_run = toml::get<bool>(data.at("dry_run"));
  
  
  if (dry_run)
  {
    //Create array of server to delimit each server config
    std::vector<toml::Table> server = toml::get<toml::Array<toml::Table> >(data.at("server"));

    //Create variables for each element of server
    std::string server_name = toml::get<toml::String>(server.at(0).at("server_name"));    
    std::string port = toml::get<toml::String>(server.at(0).at("port"));    
    std::string ip = toml::get<toml::String>(server.at(0).at("ip"));    
    std::string root_dir = toml::get<toml::String>(server.at(0).at("root_dir"));    

    std::cout << log_file << std::endl;
    std::cout << dry_run << std::endl;
    std::cout << "\n" << std::endl;
    std::cout << server_name << std::endl;
    std::cout << port << std::endl;
    std::cout << ip << std::endl;
    std::cout << root_dir <<std::endl;
    // FIXME if error in the parsing, return 2

    //return 0;
  }
  std::vector<HTTPServerOptions> servers_options;
  HTTPServerOptions options(6669, 0, "0.0.0.0");

  servers_options.push_back(options);
  ServersHandler servers_handler(servers_options);
  
  
  return 0;

}
