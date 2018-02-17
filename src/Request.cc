#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>


std::string ResponseBuilder::Request::get_token(const std::string& request, const std::string& delimiter)
{
  return request.substr(0, request.find(delimiter));
}

std::string ResponseBuilder::Request::get_request_rest(std::string& request, const std::string& delimiter)
{
  return request.erase(0, request.find(delimiter) + delimiter.length());
}

int ResponseBuilder::Request::parse_fields(std::string message_header)
{
  std::string delimiter = ":";
  std::string field = get_token(message_header, delimiter);
  if (field.empty())
    return -1;
  std::string value = get_request_rest(message_header, delimiter);
  return 0;
}

int ResponseBuilder::Request::parse_request_line(std::string request)
{
  return 0;
}

int ResponseBuilder::Request::parse_general_header(std::string message_header)
{
  if (parse_fields(message_header))
    return -1;
  return 0;
}

int ResponseBuilder::Request::parse_request_header(std::string message_header)
{
  if (parse_fields(message_header))
    return -1;
  return 0;
}

int ResponseBuilder::Request::parse_entity_header(std::string message_header)
{
  if (parse_fields(message_header))
    return -1;
  return 0;
}

int ResponseBuilder::Request::parse_body(std::string request)
{
  return 0;
}

int ResponseBuilder::Request::parse_request(std::string request)
{
  /*
  Request = Request-Line
  * (( general-header | request-header | entity-header ) CRLF )
  CRLF
  [ message-body ]
  */
  std::string delimiter = "\r\n";

  if (parse_request_line(get_token(request, delimiter)));
    return -1;

  std::string rest = get_request_rest(request, delimiter);
  while (!rest.empty())
  {
    std::string next_token = get_token(rest, delimiter);
    if (parse_general_header(next_token) == -1
        && parse_request_header(next_token) == -1
        && parse_entity_header(next_token) == -1)
      return -1;
    rest = get_request_rest(request, delimiter);
  }
  if (parse_body(get_token(request, delimiter)))
    std::cout << "Body found" << std::endl;
  return 0;
}


int ResponseBuilder::Request::get_request_type(std::string request, void*& params)
{
    request = request;
    //fixme: mutiple request types possible?
    params_ = new struct fileparams;
    ((struct fileparams*)params_)->path = "../testfile";
    type_ = RQFILE;
    return 0;
}