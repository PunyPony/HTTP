#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "ConfigOptions.hh"


class ServerConfig{
  
  public:
    ServerConfig();
    
    Serverparameter<std::string>& get_server_name();
    void set_server_name(std::string name);

    Serverparameter<std::string>& get_port();
    void set_port(std::string port);

    Serverparameter<std::string>& get_ip();
    void set_ip(std::string ip);

    Serverparameter<std::string>& get_root_dir();
    void set_root_dir(std::string root_dir);

    Serverparameter<bool>& get_gzip();
    void set_gzip(bool gzip);

    Serverparameter<bool>& get_log();
    void set_log(bool log);
  
    Serverparameter<std::string>& get_cgi_ext();
    void set_cgi_ext(std::string cgi_exit);

    Serverparameter<std::string>& get_ssl_certificate();
    void set_ssl_certificate(std::string ssl_certificate);

    Serverparameter<std::string>& get_ssl_certificate_key();
    void set_ssl_certificate_key(std::string ssl_certificate_key);
 
    Serverparameter<bool>& get_basic_auth();
    void set_basic_auth(bool basic_auth);

    Serverparameter<std::string>& get_basic_auth_file();
    void set_basic_auth_file(std::string basic_auth_file);
/*
    Serverparameter<std::vector<std::vector<std::string>>>& get_custom_error();
    void set_custom_error(std::vector<std::vector<std::string>> custom_error_);
*/
    Serverparameter<std::map<std::string, std::string>>& get_custom_error();
    void set_custom_error(std::map<std::string, std::string> custom_error_);


  private:
    
   
    Serverparameter<std::string> server_name;
    Serverparameter<std::string> port;
    Serverparameter<std::string> ip;
    Serverparameter<std::string> root_dir;
    Serverparameter<bool> gzip;
    Serverparameter<bool> log;
    Serverparameter<std::string> cgi_ext;
    Serverparameter<std::string> ssl_certificate;
    Serverparameter<std::string> ssl_certificate_key;
    Serverparameter<bool> basic_auth;
    Serverparameter<std::string> basic_auth_file;
    Serverparameter<std::map<std::string, std::string>> custom_error;
};
