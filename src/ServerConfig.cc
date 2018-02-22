#include <stdio.h>
#include <string>
#include "ServerConfig.hh"

/*
enum Param {
  "server_name",
  "port",
  "ip",
  "root_dir",
  "gzip", 
  "log",
  "cgi_ext",
  "ssl_certificate",
  "ssl_certificate_key",
  "basic_auth",
  "basic_auth_file"
};
*/
    ServerConfig::ServerConfig(){}

    
    Serverparameter<std::string>& ServerConfig::get_server_name()
    {
      return server_name;
    }

    void ServerConfig::set_server_name(std::string name)
    {
      server_name.setparam(name);
    }

    Serverparameter<std::string>& ServerConfig::get_port()
    {
      return port;
    }

    void ServerConfig::set_port(std::string port_)
    {
      port.setparam(port_);
    }

    Serverparameter<std::string>& ServerConfig::get_ip()
    {
      return ip;
    }

    void ServerConfig::set_ip(std::string ip_)
    {
      ip.setparam(ip_);
    }

    Serverparameter<std::string>& ServerConfig::get_root_dir()
    {
      return root_dir;
    }

    void ServerConfig::set_root_dir(std::string root_dir_)
    {
      root_dir.setparam(root_dir_);
    }

    Serverparameter<bool>& ServerConfig::get_gzip()
    {
      return gzip;
    }

    void ServerConfig::set_gzip(bool gzip_)
    {
      gzip.setparam(gzip_);
    }

    Serverparameter<bool>& ServerConfig::get_log()
    {
      return log;
    }

    void ServerConfig::set_log(bool log_)
    {
      log.setparam(log_);
    }
  
    Serverparameter<std::string>& ServerConfig::get_cgi_ext()
    {
      return cgi_ext;
    }

    void ServerConfig::set_cgi_ext(std::string cgi_ext_)
    {
      cgi_ext.setparam(cgi_ext_);
    }

    Serverparameter<std::string>& ServerConfig::get_ssl_certificate()
    {
      return ssl_certificate;
    }

    void ServerConfig::set_ssl_certificate(std::string ssl_certificate_)
    {
      ssl_certificate.setparam(ssl_certificate_);
    }

    Serverparameter<std::string>& ServerConfig::get_ssl_certificate_key()
    {
      return ssl_certificate_key;
    }

    void ServerConfig::set_ssl_certificate_key(std::string ssl_certificate_key_)
    {
      ssl_certificate_key.setparam(ssl_certificate_key_);
    }
 
    Serverparameter<bool>& ServerConfig::get_basic_auth()
    {
      return basic_auth;
    }

    void ServerConfig::set_basic_auth(bool basic_auth_)
    {
      basic_auth.setparam(basic_auth_);
    }

    Serverparameter<std::string>& ServerConfig::get_basic_auth_file()
    {
      return basic_auth_file;
    }

    void ServerConfig::set_basic_auth_file(std::string basic_auth_file_)
    {
      basic_auth_file.setparam(basic_auth_file_);
    }

/*
    Serverparameter<std::vector<std::vector<std::string>>>& ServerConfig::get_custom_error()
    {
      return custom_error;
    }

    void ServerConfig::set_custom_error(std::vector<std::vector<std::string>> custom_error_)
    {
      custom_error.setparam(custom_error_);
    }
*/

    Serverparameter<std::map<std::string, std::string>>& ServerConfig::get_custom_error()
    {
      return custom_error;
    }

    void ServerConfig::set_custom_error(std::map<std::string, std::string> custom_error_)
    {
      custom_error.setparam(custom_error_);
    }
