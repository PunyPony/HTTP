#pragma once
#include <string>
#include "ResponseBuilder.hh"
#include "enum.hh"

class ResponseBuilder;

class Request
{
public:
    Request(ResponseBuilder* R);
    std::string get_token(const std::string& request,
    const std::string& delimiter);
    std::string get_request_rest(std::string& request,
    const std::string& delimiter);
    int parse_fields(std::string message_header);
    std::string clean_string(std::string& s);
    void Set_field(std::string& field, std::string& value);
    int parse_request_line(std::string request);
    int parse_header(std::string message_header);
    int parse_body(std::string request);
    int parse_request(std::string request);
    int get_request_type();
   // std::string error_format(error_type err, std::string error_message);

private:
    ResponseBuilder * R_;
};