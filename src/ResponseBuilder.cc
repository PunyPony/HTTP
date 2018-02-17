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
#include <fstream>

#include <ResponseBuilder.hh>

ResponseBuilder::ResponseBuilder(int client_sock, std::string request)
{
    client_sock_ = client_sock;
    request_ = request;
}

int ResponseBuilder::analyse_request()
{
    req.get_request_type();
    req.parse_request(request_);
}

int ResponseBuilder::generate_response()
{
    res.forge_response();
}

std::string get_request(int client_sock)
{
    char buffer[4096];
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

std::string Request::get_token(const std::string& request, const std::string& delimiter)
{
  return request.substr(0, request.find(delimiter));
}

std::string Request::get_request_rest(std::string& request, const std::string& delimiter)
{
  return request.erase(0, request.find(delimiter) + delimiter.length());
}

int Request::parse_fields(std::string message_header)
{
  std::string delimiter = ":";
  std::string field = get_token(message_header, delimiter);
  if (field.empty())
    return -1;
  std::string value = get_request_rest(message_header, delimiter);
  return 0;
}

int Request::parse_request_line(std::string request)
{
  return 0;
}

int Request::parse_header(std::string message_header)
{
  if (parse_fields(message_header))
    return -1;
  return 0;
}

int Request::parse_general_header(std::string message_header)
{
  return 0;
}

int Request::parse_request_header(std::string message_header)
{
  if (parse_fields(message_header))
    return -1;
  return 0;
}

int Request::parse_entity_header(std::string message_header)
{
  if (parse_fields(message_header))
    return -1;
  return 0;
}

int Request::parse_body(std::string request)
{
  return 0;
}

int Request::parse_request(std::string request)
{
  /*
  Request = Request-Line
  * (( general-header | request-header | entity-header ) CRLF )
  CRLF
  [ message-body ]
  */
  std::string delimiter = "\r\n";
  std::cout << "Start : request is " << request << std::endl;
  if (parse_request_line(get_token(request, delimiter)))
  {
    std::cout << "request line failed " << get_token(request, delimiter) << std::endl;
    //return -1;
  }
  
  
  std::string rest = get_request_rest(request, delimiter);
  std::cout << "Rest is :" << rest << std::endl;
  while (!rest.empty())
  {
    std::string next_token = get_token(rest, delimiter);
    std::cout << "Next token" << next_token << std::endl;
    if (!parse_header(next_token) == -1)
      return -1;
    rest = get_request_rest(request, delimiter);
    std::cout << "Rest is :" << rest << std::endl;
  }
  if (!parse_body(get_token(request, delimiter)))
    std::cout << "Body found" << std::endl;

  return 0;
}

void ResponseBuilder::error(std::string msg, int code)
{
    std::cerr << msg << std::endl;
}

int Request::get_request_type()
{
    //fixme: mutiple request types possible?
    R_->params_ = new struct fileparams;
    ((struct fileparams*)R_->params_)->path = "../testfile";
    R_->type_ = RQFILE;
    return 0;
}

std::string Response::forge_error_response(error_type err)
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
    return "ERROR " + std::to_string(err) + " : " + error_message +"\n";
}

int Response::forge_response()
{
    switch (R_->type_)
    {
    case RQFILE:
    {
        struct fileparams* definedparams = (struct fileparams*)R_->params_;
        std::string path = definedparams->path;
        //fixme: check path bounds
        std::ifstream file(path);
        if (!file.good())
            R_->response_ = forge_error_response(FILE_NOT_FOUND); //fixme: check error type(not found, cannot open...)
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
        break;
    }
    default:
        R_->response_ = forge_error_response(NIQUE_TA_MERE); //or INTERNAL_ERROR for less fun
    }
}

int ResponseBuilder::send_reponse()
{
    int res = write(client_sock_, &response_[0], response_.size()); //should we write <end of file>?
    if (res == -1)
        error("write", 2);
    else
        if ((size_t)res != response_.size())
            error("write: wrong size written", 2);
    return 0;
}