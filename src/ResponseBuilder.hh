#pragma once
#include <string>
#include <unordered_map>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <memory>
#include "SynchronizedFile.hh"
#include "HTTPServerOptions.hh"
#include "Request.hh"
#include "Response.hh"
#include "enum.hh"

class ResponseBuilder;

std::string get_request(int client_sock);

class ResponseBuilder {
public:
    ResponseBuilder(int client_sock, std::string request, HTTPServerOptions& options, std::shared_ptr<SynchronizedFile>& log_file);
    int analyse_request();
    int generate_response();
    int send_reponse();
    int log();
    std::string& get_response();
    void set_response(std::string& response);
protected:
    friend class Request;
    friend class Response;
    int client_sock_;
    enum request_type type_ = UNKNOWN;
    enum error_type parsing_error_ = NICEUH;
    std::string requested_ressource_;
    std::string version_;
    std::string request_;
    std::string response_;
    HTTPServerOptions& options_;
    std::shared_ptr<SynchronizedFile> log_file_;
    std::unordered_map<std::string, std::string> fields_;
private:
    void error(std::string msg, int code);
    Request req{this};
    Response res{this};
};
