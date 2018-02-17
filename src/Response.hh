#pragma once
#include <string>
#include <ResponseBuilder.hh>

class ResponseBuilder;
class Response
{
   //Response();
   void error(std::string msg, int code);
   std::string forge_error_response(error_type err);
   int forge_response();
  std::string response_;
   void send_reponse(int client_sock, std::string response);
};