#include <iostream>
#include <fstream>
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
#include <algorithm>

#include "ResponseBuilder.hh"
#include "Request.hh"

Request::Request(ResponseBuilder* R) 
{ 
    R_ = R; 
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
    std::cout << "field = "<< field << std::endl;
    if (field.empty())
        return -1;
    std::string value = get_request_rest(message_header, delimiter);
    std::cout << "value = "<< value << std::endl;
    field = clean_string(field);
    Set_field(field, value);
    return 0;
}

void Request::Set_field(std::string& field, std::string& value)
{
  std::pair<std::string, std::string>  el(field, value);
  R_->fields_.insert(el);
  std::cout << "Inserted : " << el.first << " : " << el.second << std::endl;
}

std::string Request::clean_string(std::string& s)
{
  remove_if(s.begin(), s.end(), isspace);
  std::transform(s.begin(), s.end(), s.begin(),
  [](unsigned char c){ return std::toupper(c);});
  return s;
}

int Request::parse_request_line(std::string request_line)
{
    std::string delimiter = " ";
    std::string method = get_token(request_line, delimiter);
    std::cout << "method = " << method << std::endl;
    method = clean_string(method);
    std::cout << "clean method = " << method << std::endl;

    //fixme

    get_request_rest(request_line, delimiter);
    std::string request_uri = get_token(request_line, delimiter);
    std::cout << "request_uri = " << request_uri << std::endl;

    get_request_rest(request_line, delimiter);
    std::string http_version = get_token(request_line, delimiter);
    std::cout << "http_version = " << http_version << std::endl;

    R_->version_ = http_version;

    if (method.empty() || request_uri.empty() || http_version.empty())
    {
        R_->parsing_error_ = NIQUE_TA_MERE; //fixme
        return -1;
    }

    if (http_version != "HTTP/1.1" && http_version != "HTTP/1.0")
    {
        R_->parsing_error_ = HTTP_VERSION_NOT_SUPPORTED;
        return -1;
    }

    if (method == "GET")
    {
        R_->type_ = GET;
        R_->requested_ressource_ = request_uri;
        /*
        R_->params_ = new struct fileparams;
        ((struct fileparams*)R_->params_)->path = request_uri;
        */
    }
    else if (method == "POST")
    {
        R_->type_ = POST;
        //fixme
    }
    return 0;
}

int Request::parse_header(std::string message_header)
{
    if (parse_fields(message_header))
        return -1;
    return 0;
}

int Request::parse_body(std::string body)
{
    body = body;
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
    std::cout << "Request = " << request << std::endl;
    if (parse_request_line(get_token(request, delimiter)))
    {
        std::cout << "request line failed " << get_token(request, delimiter) << std::endl;
        return -1;
    }


    std::string rest = get_request_rest(request, delimiter);
    std::cout << "Rest = " << rest << std::endl;
    while (!rest.empty())
    {
      std::string next_token = get_token(rest, delimiter);
      std::cout << "Next token = " << next_token << std::endl;
      if (!parse_header(next_token))
        return -1;
      rest = get_request_rest(request, delimiter);
      std::cout << "Rest = " << rest << std::endl;
    }
    if (!parse_body(get_token(request, delimiter)))
      std::cout << "Body found = " << std::endl;

    return 0;
}