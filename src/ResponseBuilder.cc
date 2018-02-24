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

// TO PLACE IN HEADER BUT FLEME
std::string error_format(error_type err, std::string error_message);



ResponseBuilder::ResponseBuilder(int client_sock, std::string request, HTTPServerOptions& options, std::shared_ptr<SynchronizedFile>& log_file)
    :options_(options)
{
    log_file_ = log_file;
    client_sock_ = client_sock;
    request_ = std::move(request);
    //sleep(1);
}

int ResponseBuilder::log()
{
    std::string serv_name = options_.get_server_tab().get_server_name().getparam();
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

int ResponseBuilder::analyse_request()
{
    req.parse_request(request_);
    return 0;
}

int ResponseBuilder::generate_response()
{
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

void ResponseBuilder::error(std::string msg, int code)
{
    std::cerr << msg << " : " << code << std::endl;
}

/*
int Request::get_request_type()
{
    //fixme: mutiple request types possible?
    R_->params_ = new struct fileparams;
    ((struct fileparams*)R_->params_)->path = "/home/nicolas/projects/MyHTTPD/testfile";
    R_->type_ = GET;
    R_->requested_ressource_ = "/home/nicolas/projects/MyHTTPD/testfile";
    return 0;
}*/

std::string Response::forge_error_response(error_type err)
{
    std::string error_message;
    switch (err)
    {
    case ACCESS_DENIED:
        error_message = "Unauthorized"; //401
        break;
    case FORBIDDEN:
        error_message = "Forbidden"; //403
        break;
    case FILE_NOT_FOUND:
        error_message = "Not Found"; //404
        break;
    case METHOD_NOT_ALLOWED:
        error_message = "Method Not Allowed"; //405
        break;
    case INTERNAL_ERROR:
        error_message = "Internal Server Error"; //500
        break;
    case HTTP_VERSION_NOT_SUPPORTED:
        error_message = "HTTP Version not supported"; //505
        break;
    case NIQUE_TA_MERE:
        error_message = "NIQUE_TA_MERE";
        break;
    case NOT_IMPLEMENTED:
        error_message = "NOT_IMPLEMENTED";
        break;
    default:
        error_message = "A + DANS LE BUS";
    }
    return error_format(err, error_message);
}

std::string formatGETanswer(std::string file) {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = gmtime(&rawtime);
    char buffer[100];

    //Protocol StatusCode ReasonPhrase
    std::string ans;
    std::string protocol("HTTP/1.1 "); //FIXME by real protocol variable
    std::string statuscode("200 "); //FIXME by real statuscode variable
    std::string reasonphrase("OK\n"); //FIXME by real reasonphrase variable
    ans = protocol + statuscode + reasonphrase;

    //Date
    ans.append("Date: ");
    strftime(buffer, 100, "%a, %d %b %G %T GMT", timeinfo);
    std::string tmp(buffer);
    ans.append(tmp);
    ans.append("\n");

    //File Length
    ans.append("Content-length: ");
    std::string length = std::to_string(file.length());
    ans.append(length);
    ans.append("\n\n"); //FIXME to remove /n/n if other header afterward

                        //Content
    ans.append(file);

    return ans;
}


std::string Response::error_format(error_type err, std::string error_message)
{
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = gmtime(&rawtime);
    char buffer[100];

    //Protocol StatusCode ReasonPhrase
    std::string ans;
    std::string statuscode(std::to_string(err) + " "); 
    std::string reasonphrase(error_message + "\n"); 
    ans = R_->version_ + " " + statuscode + reasonphrase;

    //Date
    ans.append("Date: ");
    strftime(buffer, 100, "%a, %d %b %G %T GMT", timeinfo);
    std::string tmp(buffer);
    ans.append(tmp);
    ans.append("\n\n");

    //Check if custom error page exist and return them
    if (R_->options_.get_server_tab().get_custom_error().get())
    {
      std::map<std::string, std::string> test = R_->options_.get_server_tab().get_custom_error().getparam();
      for (const auto& t : test)
      {
        if (std::stoi(t.first) == err)
        {
          std::ifstream file(t.second);
          if (file.good())
          {
            std::string custom;
            //file >> content;

            file.seekg(0, std::ios::end);
            custom.reserve(file.tellg());
            file.seekg(0, std::ios::beg);

            custom.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
            ans.append(custom);
          }

        }
      }
    }


    return ans;

}

int Response::forge_response()
{
    if (R_->parsing_error_ != NICEUH) {
        R_->response_ = forge_error_response(R_->parsing_error_);
        return 0;
    }
    switch (R_->type_)
    {
    case UNKNOWN:
        R_->response_ = forge_error_response(METHOD_NOT_ALLOWED);
        break;
    case GET:
    {
        //struct fileparams* definedparams = (struct fileparams*)R_->params_;
        std::string path = R_->options_.get_server_tab().get_root_dir().getparam() + R_->requested_ressource_; // definedparams->path;
        struct stat st;
        if (-1 == stat(path.c_str(), &st))
        {
            //fixme
        }
        else
        {
            if (S_ISDIR(st.st_mode))
            {
                if (path.back() != '/')
                    path.append("/");
                path.append("index.html");
            }
        }
        //fixme: check path bounds
        std::ifstream file(path);
        if (!file.good())
            R_->response_ = forge_error_response(FILE_NOT_FOUND);
        else
        {
            std::string content;
            //file >> content;

            file.seekg(0, std::ios::end);
            content.reserve(file.tellg());
            file.seekg(0, std::ios::beg);

            content.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
            R_->response_ = formatGETanswer(content);
        }
        //delete definedparams;
        break;
    }
    case POST:
        R_->response_ = forge_error_response(NOT_IMPLEMENTED);
        break;
    default:
        R_->response_ = forge_error_response(NIQUE_TA_MERE); //or INTERNAL_ERROR for less fun
    }
    return 0;
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
