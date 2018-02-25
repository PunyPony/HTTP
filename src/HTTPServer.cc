#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <thread>

#include "ResponseBuilder.hh"
#include "HTTPServer.hh"
#include "HTTPServerOptions.hh"
#include "ThreadPool.hh"

HTTPServer::HTTPServer(std::unordered_map<std::string, HTTPServerOptions> options, std::string log_file_path)
    : options_(options), log_file_(std::make_shared<SynchronizedFile>(log_file_path))
{
    // Insert a filter to fail all the CONNECT request, if required
    // Add Content Compression filter (gzip), if needed. Should be
    // final filter
}

HTTPServer::~HTTPServer()
{}

//END of code to move

int HTTPServer::init(int& sock) //fix les acc�s de merde
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        //std::cout << "sock fail" << std::endl;
        return -1;
    }
    struct sockaddr_in addrin;
    addrin.sin_family = AF_INET;
    addrin.sin_port = htons(atoi(options_.begin()->second.get_server_tab().get_port().getparam().c_str()));

    //convert string ip to good form
    if (!inet_aton(options_.begin()->second.get_server_tab().get_ip().getparam().c_str(), &addrin.sin_addr))
    {
        //std::cout << "inet_aton fail" << std::endl;
        return -1;
    }
    if (bind(sock, (sockaddr*)&addrin, sizeof(addrin)) == -1)
    {
        //std::cout << "bind fail" << std::endl;
        return -1;
    }
    if (listen(sock, 10) == -1) //limited to 10 connections ; must be an option
    {
        //std::cout << "listen fail" << std::endl;
        return -1;
    }
    return 0;
}

int HTTPServer::start(int sock)
{
    int epollfd = epoll_create1(0);
    auto tmp = std::thread([sock, epollfd]() {
        for (;;)
        {
            struct sockaddr_in sockin;
            socklen_t socklen = sizeof(struct sockaddr_in);
            int client_sock = accept4(sock, (sockaddr*)&sockin, &socklen, SOCK_NONBLOCK);
            if (client_sock != -1)
            {
                //std::cout << "New client" << std::endl;
                struct epoll_event tmpevents {
                    EPOLLIN,
                    { 0 }
                };
                tmpevents.data.fd = client_sock;
                if (-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, client_sock, &tmpevents))
                {
                    //std::cout << "epoll_ctl error" << std::endl;
                    return -1;
                }
            }
        }
    });
    struct epoll_event events[10]{ 0, {0} };
    for (;;)
    {
        int waitres = epoll_wait(epollfd, events, 10, -1); //nb of events?
        for (int i = 0; i < waitres; i++)
        {
            int requested_sock = events[i].data.fd;
            std::string request = get_request(requested_sock);
            if (!request.size())
            {
                if (-1 == epoll_ctl(epollfd, EPOLL_CTL_DEL, requested_sock, NULL)) {
                    //fixme: log error
                }
                continue;
            }
            DefaultThreadPool::submitJob([requested_sock, request, this, epollfd]() //fixme: sock where event occured
            {
                ResponseBuilder builder(requested_sock, request, get_log_file());

                builder.analyse_request();
                HTTPServerOptions* options;
                if (this->get_server_count() == 1)
                    options = this->get_server_options();
                else
                    options = this->get_server_options(builder.get_request_header("SERVER_NAME"));
                if (options)
                    builder.set_options(options);
                auto found = GlobalCache::getCache().cache_.find(request);
                if (found == GlobalCache::getCache().cache_.end())
                {                    
                    // start of analyse
                    builder.analyse_request();
                    // build response
                    builder.generate_response();
                    // cache
                    //std::cout << "Not cached" << std::endl;
                    auto response = builder.get_response();
                    GlobalCache::getCache().insert(std::pair<std::string, std::string>(request, response));
                }
                else
                {
                    //std::cout << "Cached" << std::endl;
                    builder.set_response(found->second);
                }
                // write to log file
                if (options && options->get_server_tab().get_log().getparam())
                    builder.log();
                // send response
                if (builder.send_reponse() == 1) //connection closed (by client or version is HTTP/1.0)
                    if (-1 == epoll_ctl(epollfd, EPOLL_CTL_DEL, requested_sock, NULL))
                    {
                        //fixme: log error
                    }
            });
        }
    }

    close(sock);
    return 0;
}

//HTTPServerOptions& HTTPServer::get_options()
//{
//    return options_;
//}

HTTPServerOptions* HTTPServer::get_server_options(std::string server_name)
{
    auto res = options_.find(server_name);
    if (res != options_.end())
        return &res->second;
    return nullptr;
}

HTTPServerOptions* HTTPServer::get_server_options()
{
    return &options_.begin()->second;
}

int HTTPServer::get_server_count()
{
    return options_.size();
}

std::shared_ptr<SynchronizedFile>& HTTPServer::get_log_file()
{
    return log_file_;
}

void HTTPServer::stopListening()
{
}

void HTTPServer::stop()
{
    stopListening();
}
