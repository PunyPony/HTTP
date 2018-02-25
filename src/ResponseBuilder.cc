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
#include "Response.hh"
#include "Request.hh"

ResponseBuilder::ResponseBuilder(int client_sock, std::string request, std::shared_ptr<SynchronizedFile>& log_file)
{
    log_file_ = log_file;
    client_sock_ = client_sock;
    request_ = std::move(request);
    /*res = this;
    req = this;*/
    //sleep(1);
}

std::string& ResponseBuilder::get_response()
{
    return response_;
}

void ResponseBuilder::set_response(std::string& response)
{
    response_= response;
}

int ResponseBuilder::log()
{
    std::string serv_name = options_->get_server_tab().get_server_name().getparam();
    //"[NAME_SERV] VERSION_HTTP REQUEST_TYPE RESOURCE_REQUESTED"
    std::string type;
    switch (type_)
    {
        case 1:
            type = "GET";
            break;
        case 2:
            type = "POST";
            break;
        default:
            type = "UNKNOW";
            break;
    }

    std::string log_line = "[" + serv_name + "]"+ " " + version_ + " " + type + " " + requested_ressource_ + "\n";
    std::cout << "Log Line : " << log_line << std::endl;
    log_file_->write(log_line);
    return 0;
}

std::string ResponseBuilder::get_request_header(std::string name)
{
    return fields_[name];
}

void ResponseBuilder::set_options(HTTPServerOptions* options)
{
    options_ = options;
}

int ResponseBuilder::analyse_request()
{
    req.parse_request(request_);
    return 0;
}

int ResponseBuilder::generate_response()
{
    if (options_ == nullptr)
        parsing_error_ = FORBIDDEN; //fixme: move this check
    res.forge_response();
    return 0;
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

int ResponseBuilder::send_reponse()
{
    int res = send(client_sock_, &response_[0], response_.size(), MSG_NOSIGNAL); //should we write <end of file>?
    if (res == -1)
    {
        error("write", 2);
        if (errno == EPIPE)
            return 1; //connection closed
        return -1; //unknown error
    }
    else
        if ((size_t)res != response_.size())
            error("write: wrong size written", 2);
    if (version_ == "HTTP/1.0")
    {
        close(client_sock_);
        return 1;
    }
    return 0;
}
