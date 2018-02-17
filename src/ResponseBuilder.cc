#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <ResponseBuilder.hh>

ResponseBuilder::ResponseBuilder()
{}

inline std::string get_request(int client_sock)
{
    char *buffer;
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