#pragma once
#include <string>
#include <Response.hh>
#include <Request.hh>


//class Response;
//class Request;

std::string get_request(int client_sock);

class ResponseBuilder {
 private:
  enum request_type
  {
    RQFILE
  };
  enum error_type
  {
    ACCESS_DENIED = 401,
    FILE_NOT_FOUND = 404,
    INTERNAL_ERROR = 500,
    NIQUE_TA_MERE = 6969
  };
  struct fileparams
  {
    std::string path;
  };
 public:
 ResponseBuilder();
  friend class Request;
  friend class Response;
 private:
  int client_sock_;
  enum request_type type_;
  void* params_;
  std::string request_;

};

