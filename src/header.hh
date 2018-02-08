#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <vector>
//#include <string>
//#define _DEFAULT_SOURCE
//#define _GNU_SOURCE

#define THREADS_NB 3

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

void error(std::string, int);
int init_connection();
void init_threads();
int wait_for_client(int sock);
std::string get_request(int client_sock);
void dispatch_request(int client_sock, std::string request);
request_type get_request_type(std::string request, void*& params);
std::string forge_error_response(error_type err);
std::string forge_response(request_type, void* params);
void send_reponse(int sock, std::string response);

extern std::vector<struct threadinfo> g_threads;
