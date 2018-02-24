#pragma once
#include <string>
#include "ResponseBuilder.hh"
#include "enum.hh"

class ResponseBuilder;

class Response
{
public:
    Response(ResponseBuilder* R);
    std::string forge_error_response(error_type err);
    std::string error_format(error_type err, std::string error_message);
    std::string formatGETanswer(std::string file);
    int forge_response();
private:
    ResponseBuilder * R_;
};