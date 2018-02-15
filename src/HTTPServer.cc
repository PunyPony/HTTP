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
    : options_(options)
{
    // Insert a filter to fail all the CONNECT request, if required
    // Add Content Compression filter (gzip), if needed. Should be
    // final filter
}

HTTPServer::~HTTPServer()
{}

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
//START of code to move
void error(std::string msg, int code)
{
    std::cerr << msg << std::endl;
    exit(code);
}

enum error_type
{
    ACCESS_DENIED = 401,
    FILE_NOT_FOUND = 404,
    INTERNAL_ERROR = 500,
    NIQUE_TA_MERE = 6969
};

enum request_type
{
    RQFILE
};

struct fileparams
{
    std::string path;
};

struct threadinfo
{
    pthread_t th;
    bool is_active;
    int client_sock;
    std::string request;
};

std::string yologet_request(int client_sock)
{
    size_t blocksize = 500;
    size_t totalsize = 0;
    std::string request;
    int res = 0;
    while (res != -1)
    {
        request.reserve(totalsize + blocksize);
        request.resize(totalsize + blocksize);
        res = read(client_sock, &request[totalsize], blocksize);
        totalsize += res;
    }
    request.resize(totalsize + 1);
    return request;
}

request_type get_request_type(std::string request, void*& params)
{
    request = request;
    //fixme: mutiple request types possible?
    params = new struct fileparams;
    ((struct fileparams*)params)->path = "../testfile";
    return RQFILE;
}
std::string forge_error_response(error_type err)
{
    return "ERROR " + std::to_string(err) + ": ?a marche pas lol \n";
}
std::string forge_response(request_type rqtype, void* params)
{
    std::string result;
    switch (rqtype)
    {
    case RQFILE:
        struct fileparams* definedparams = (struct fileparams*)params;
        std::string path = definedparams->path;
        //fixme: check path bounds
        std::ifstream file(path);
        if (!file.good())
            result = forge_error_response(FILE_NOT_FOUND); //fixme: check error type(not found, cannot open...)
        else
        {
            std::string content;
            file >> content;

            file.seekg(0, std::ios::end);
            content.reserve(file.tellg());
            file.seekg(0, std::ios::beg);

            content.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
            result = content;
        }
        delete definedparams;
        return result;
    }
    return forge_error_response(NIQUE_TA_MERE); //or INTERNAL_ERROR for less fun
}
void send_reponse(int client_sock, std::string response)
{
    int res = write(client_sock, &response[0], response.size()); //should we write <end of file>?
    if (res == -1)
        error("write", 2);
    else
        if ((size_t)res != response.size())
            error("write: wrong size written", 2);
}

//END of code to move

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
    struct epoll_event events[10]{ 0 };
    int debug_clientsock; //debug variable to test for 1 client (last connected)
    for (;;)
    {
        struct sockaddr_in sockin;
        socklen_t socklen = sizeof(struct sockaddr_in);
        int client_sock = accept4(sock, (sockaddr*)&sockin, &socklen, SOCK_NONBLOCK);
        if (client_sock != -1)
        {
            std::cout << "New client" << std::endl;
            struct epoll_event tmpevents { 
                EPOLLIN,
                { .fd = client_sock }
            };
            if (-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, client_sock, &tmpevents))
            {
                std::cout << "epoll_ctl error" << std::endl;
                return -1;
            }
            debug_clientsock = client_sock;
        }
        int waitres = epoll_wait(epollfd, events, 10, 0); //nb of events?
        for(int i = 0; i < waitres; i++)
        {
            client_sock = events[i].data.fd;
            std::string request = yologet_request(client_sock);
            DefaultThreadPool::submitJob([client_sock, request]() //fixme: sock where event occured
            {
                std::cout << "c partit ma grosse bite!" << std::endl;
                //start of analyse
                void* params = NULL;
                request_type rqtype = get_request_type(request, params);
                std::string response = forge_response(rqtype, params);

                std::cout << "response is" << std::endl << response << "end of response" << std::endl;
                std::cout << "sending response:..." << std::endl;

                send_reponse(client_sock, response);

                std::cout << "done" << std::endl;
                //close(client_sock); //fixme: should we close this
                                         //end of analyse
            }
                /*
            {
                char buffer[200] = { 0 };
                int ret = recv(client_sock, buffer, 199, 0);
                if (ret == 0 || ret == -1)
                {
                    //std::cout << "Error recv" << std::endl;
                    return -1;
                }
                std::cout << "New request from client: " << buffer << std::endl;
                ret = send(client_sock, buffer, ret, 0);
                if (ret == 0 || ret == -1)
                {
                    std::cout << "error send" << std::endl;
                }
            }
            */
            );
        }
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
