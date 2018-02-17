#pragma once
#include <string>
#include <ResponseBuilder.hh>

class ResponseBuilder;
class Request 
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