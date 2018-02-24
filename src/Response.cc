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

Response::Response(ResponseBuilder* R) 
{ 
    R_ = R; 
}

void ResponseBuilder::error(std::string msg, int code)
{
    std::cerr << msg << " : " << code << std::endl;
}


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

std::string Response::formatGETanswer(std::string file)
{
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