#include <stdio.h>
#include <../toml/toml.hpp>
#include <fstream>
#include <string>

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

  toml::Data data = toml::parse(file);

  std::string server_name = toml::get<toml::String>(data.at(server."server_name"));
  std::string port = toml::get<toml::String>(data.at("port"));
  std::string ip = toml::get<toml::String>(data.at("ip"));
  std::string root_dir = toml::get<toml::String>(data.at("root_dir"));


  std::cout << server_name <<std::endl;
  std::cout << port <<std::endl;
  std::cout << ip <<std::endl;
  std::cout << root_dir <<std::endl;

  return 0;
}
