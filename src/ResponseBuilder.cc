#include <ResponseBuilder.hh>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
//#include <cstdlib>
//#include <cstring>
#include <unistd.h>

ResponseBuilder::ResponseBuilder()
{}

inline std::string get_request(int client_sock)
{
    char *buffer;
    int BUFFER_SIZE = 4096;
    bzero(buffer, BUFFER_SIZE);
    std::stringstream readStream;
    int readResult = 0;
    std::cout << "Receiving chunk... ";
    while ((readResult = read(client_sock, buffer, BUFFER_SIZE)) > 0)
      readStream << buffer;

    if (readResult == 0)
    {
      // Timeout - handle that. You could try waiting again, close the socket...
      //return 1;
    }
    if (readResult == -1)
    {
      // Handle the error
      //return 1;
    }

    std::string readData = readStream.str();
    std::cout << readData << std::endl;
    return readData;
}

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

void ResponseBuilder::Response::error(std::string msg, int code)
{
    std::cerr << msg << std::endl;
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

std::string ResponseBuilder::Response::forge_error_response(error_type err)
{
  std::string error_message;
  switch (err)
  {
    case ACCESS_DENIED:
      error_message = "ACCESS_DENIED";
      break;
    case FILE_NOT_FOUND:
      error_message = "FILE_NOT_FOUND";
      break;
    case INTERNAL_ERROR:
      error_message = "INTERNAL_ERROR";
      break;

  }
    return "ERROR " + std::to_string(err) + error_message +"\n";
}

int ResponseBuilder::Response::forge_response()
{
    switch (type_)
    {
    case RQFILE:
        struct fileparams* definedparams = (struct fileparams*)params_;
        std::string path = definedparams->path;
        //fixme: check path bounds
        std::ifstream file(path);
        if (!file.good())
            resonse_ = forge_error_response(FILE_NOT_FOUND); //fixme: check error type(not found, cannot open...)
        else
        {
            std::string content;
            file >> content;

            file.seekg(0, std::ios::end);
            content.reserve(file.tellg());
            file.seekg(0, std::ios::beg);

            content.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
            response_ = content;
        }
        delete definedparams;
    }
    response_ = forge_error_response(); //or INTERNAL_ERROR for less fun
}

void ResponseBuilder::Response:send_reponse(std::string response)
{
    int res = write(client_sock_, &response[0], response.size()); //should we write <end of file>?
    if (res == -1)
        error("write", 2);
    else
        if ((size_t)res != response.size())
            error("write: wrong size written", 2);
}


