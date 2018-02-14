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

std::string get_request(int socketFileDescriptor)
{
    char *buffer;
    int BUFFER_SIZE = 4096;
    bzero(buffer, BUFFER_SIZE);
    std::stringstream readStream;
    int readResult = 0;
    std::cout << "Receiving chunk... ";
    while ((readResult = read(socketFileDescriptor, buffer, BUFFER_SIZE)) > 0)
      readStream << buffer;

    if (readResult == 0)
    {
      // Timeout - handle that. You could try waiting again, close the socket...
    }
    if (readResult == -1)
    {
      // Handle the error
    }

    std::string readData = readStream.str();
    std::cout << readData << std::endl;
    return readData;
}

int parse_request_line(std::string request)
{
  return 0;
}

int parse_general_header(std::string request)
{
  return 0;
}

int parse_request_header(std::string request)
{
  return 0;
}

int parse_entity_header(std::string request)
{
  return 0;
}

int parse_body(std::string request)
{
  return 0;
}

std::string get_token(const std::string& request, const std::string& delimiter)
{
  return request.substr(0, request.find(delimiter));
}

std::string get_request_rest(std::string& request, const std::string& delimiter)
{
  return request.erase(0, request.find(delimiter) + delimiter.length());
}

int parse_request(std::string request)
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

  std::string rest = get_token(get_request_rest(request, delimiter), delimiter);
  while (!rest.empty())
  {
    if (parse_general_header(rest) == -1
        && parse_request_header(rest) == -1
        && parse_entity_header(rest) == -1)
      return -1;
    std::string rest = get_token(get_request_rest(request, delimiter), delimiter);
  }
  if (parse_body(get_token(get_request_rest(request, delimiter), delimiter)))
    std::cout << "Body found" << std::endl;

  return 0;
}

