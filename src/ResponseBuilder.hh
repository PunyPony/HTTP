#pragma once
#include <string>


//class Response;
//class Request;

std::string get_request(int client_sock);

class ResponseBuilder;

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

class Request
{
public:
    Request(ResponseBuilder* R) { R_ = R; }
    std::string get_token(const std::string& request, const std::string& delimiter);
    std::string get_request_rest(std::string& request, const std::string& delimiter);
    int parse_fields(std::string message_header);
    int parse_request_line(std::string request);
    int parse_general_header(std::string message_header);
    int parse_request_header(std::string message_header);
    int parse_entity_header(std::string message_header);
    int parse_body(std::string request);
    int parse_request(std::string request);
    int get_request_type();
private:
    ResponseBuilder * R_;
};

class Response
{
public:
    Response(ResponseBuilder* R) { R_ = R; }
    std::string forge_error_response(error_type err);
    int forge_response();
    std::string response_;
private:
    ResponseBuilder * R_;
};

class ResponseBuilder {
protected:
    friend class Request;
    friend class Response;
public:
    ResponseBuilder(int client_sock, std::string request);
    int analyse_request();
    int generate_response();
    int send_reponse();
private:
    void error(std::string msg, int code);

protected:
    int client_sock_;
    enum request_type type_;
    void* params_;
    std::string request_;
    std::string response_;
private:
    Request req{ this };
    Response res{ this };
};