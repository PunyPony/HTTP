#pragma once
#include <string>

std::string get_request(int client_sock);

class ResponseBuilder {
 protected:
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

  class Request friend ResponseBuilder
  {
    //Request();
    std::string get_token(const std::string& request, const std::string& delimiter);
    std::string get_request_rest(std::string& request, const std::string& delimiter);
    int parse_fields(std::string message_header);
    int parse_request_line(std::string request);
    int parse_general_header(std::string message_header);
    int parse_request_header(std::string message_header);
    int parse_entity_header(std::string message_header);
    int parse_body(std::string request);
    int parse_request(std::string request);
    int get_request_type(std::string request, void*& params);
  };

 class Response friend ResponseBuilder
 {
   //Response();
   void error(std::string msg, int code);
   std::string forge_error_response(error_type err);
   int forge_response();
  std::string response_;
   void send_reponse(int client_sock, std::string response);
 };
 protected:
  int client_sock_;
  enum request_type type_;
  void* params_;
  std::string request_;

};

