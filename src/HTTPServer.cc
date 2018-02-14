//#include <fstream>
//#include <vector>
//#include <stdlib.h>
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
//#include <socket>


#include <HTTPServer.hh>
#include <HTTPServerOptions.hh>

HTTPServer::HTTPServer(HTTPServerOptions options)
    : options_(options), request_handler_(options.threads_)
{
    // Insert a filter to fail all the CONNECT request, if required
    // Add Content Compression filter (gzip), if needed. Should be
    // final filter
}

HTTPServer::~HTTPServer() {}

class HandlerCallbacks {
public:
    /* pourquoi pas
    void threadStarted(ThreadPoolExecutor::ThreadHandle* h) override
    {}
    void threadStopped(ThreadPoolExecutor::ThreadHandle* h) override
    {}
    */
private:
};


int HTTPServer::start()
{
    int sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (sock == -1)
    {
        std::cout << "sock fail" << std::endl;
        return -1;
    }
    struct sockaddr_in addrin;
    addrin.sin_family = AF_INET;
    addrin.sin_port = htons(options_.listen_port_);

    //convert string ip to good form
    if (!inet_aton(options_.ip_.c_str(), &addrin.sin_addr))
    {
        std::cout << "inet_aton fail" << std::endl;
        return -1;
    }
    if (bind(sock, (sockaddr*)&addrin, sizeof(addrin)) == -1)
    {
        std::cout << "bind fail" << std::endl;
        return -1;
    }
    if (listen(sock, 10) == -1) //limited to 10 connections ; must be an option
    {
        std::cout << "listen fail" << std::endl;
        return -1;
    }

    int epollfd = epoll_create1(0);
    struct epoll_event events {
        EPOLLIN,
        { 0 }
    };
    int debug_clientsock; //debug variable to test for 1 client (last connected)
    for (;;)
    {
        struct sockaddr_in sockin;
        socklen_t socklen = sizeof(struct sockaddr_in);
        int client_sock = accept4(sock, (sockaddr*)&sockin, &socklen, SOCK_NONBLOCK);
        if (client_sock != -1)
        {
            std::cout << "New client" << std::endl;
            if (-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, client_sock, &events))
            {
                std::cout << "epoll_ctl error" << std::endl;
                return -1;
            }
            debug_clientsock = client_sock;
        }
        int waitres = epoll_wait(epollfd, &events, 1, 0); //nb of events?
        if (waitres > 0)
            client_sock = debug_clientsock;
            DefaultThreadPool::submitJob([client_sock]() //fixme: sock where event occured
        {
            char buffer[200] = { 0 };
            int ret = recv(client_sock, buffer, 199, 0);
            if (ret == 0 || ret == -1)
            {
                std::cout << "Error recv" << std::endl;
                return -1;
            }
            std::cout << "New request from client: " << buffer << std::endl;
            ret = send(client_sock, buffer, ret, 0);
            if (ret == 0 || ret == -1)
            {
                std::cout << "error send" << std::endl;
            }
        }
        );
    }

    close(sock);
    return 0;

}

void HTTPServer::stopListening()
{}

void HTTPServer::stop()
{
    stopListening();
}
