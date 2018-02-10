//#include <fstream>
//#include <vector>
//#include <stdlib.h>
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
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
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1)
  {
    std::cout << "sock fail" << std::endl;
    return -1;
  }
  struct sockaddr_in addrin;
  addrin.sin_family = AF_INET;
  addrin.sin_port = htons(options_.listen_port_);
    
  //convert string ip to good form
  if(!inet_aton(options_.ip_.c_str(), &addrin.sin_addr))
  {
    std::cout << "inet_aton fail" << std::endl;
    return -1;
  }
  if(bind(sock, (sockaddr*)&addrin, sizeof(addrin)) == -1)
  {
    std::cout << "bind fail" << std::endl;
    return -1;
  }
  if(listen(sock, 10) == -1) //limited to 10 connections ; must be an option
  {
    std::cout << "listen fail" << std::endl;
    return -1;
  }

  for(;;)
  {
    struct sockaddr_in sockin;
    socklen_t socklen = sizeof(struct sockaddr_in);
    int connection_sock = accept4(sock,(sockaddr*)&sockin, &socklen, 0); // SOCK_NONBLOCK);
    if(connection_sock == -1)
    {
      std::cout << "accept4 fail" << std::endl;
      return -1;
    }
    std::cout << "New client" << std::endl;
    auto future = request_handler_.getThreadPool().submit([connection_sock]()
        {
          for(;;)
          {
            char buffer[200] = {0};
            int ret = recv(connection_sock, buffer, 199, 0);
            if (ret == 0 || ret == -1)
            {
              std::cout << "End of request" << std::endl;
              break;
            }
            std::cout << "New request from client: " << buffer << std::endl;
            ret = send(connection_sock, buffer, ret, 0);
            if (ret == 0 || ret == -1)
            {
              break;
            }
          }
            });
    /*
    for(;;)
			{
				char buffer[200] = {0};
				int ret = recv(connection_sock, buffer, 199, 0);
				if (ret == 0 || ret == -1)
        {
          std::cout << "End of request" << std::endl;
					break;
        }
        std::cout << "New request from client: " << buffer << std::endl;
				ret = send(connection_sock, buffer, ret, 0);
				if (ret == 0 || ret == -1)
        {
					break;
        }
			}
    */
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
