#include "header.hh"

int listen_port = 6666;

int main(int argc, char** argv)
{
    if (argc > 1)
        listen_port = atoi(argv[1]);
    int sock = init_connection();
    init_threads();
    while (1)
    {
        int client_sock = wait_for_client(sock);

        std::string request = get_request(client_sock);
        if (request == "TA GUEULE\r\n")
            break;
        std::cout << "New request from client: " << std::endl;
        std::cout << request << "End of request" << std::endl;

        dispatch_request(client_sock, request);
    }

    close(sock);
}

//fixme replace error fct call with error message where needed (internal error: 5**)
void error(std::string msg, int code)
{
    std::cerr << msg << std::endl;
    exit(code);
}

int init_connection()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error("sock", 1);
    /*
    struct sockaddr_in addrin
     {
     .sin_family = AF_INET,
     .sin_port = htons(listen_port),
         .sin_addr = {}
     };

     */

    struct sockaddr_in addrin;
    addrin.sin_family = AF_INET;
    addrin.sin_port = htons(listen_port);


    if (!inet_aton("0.0.0.0", &addrin.sin_addr))
        error("inet_aton", 1);
    if (bind(sock, (sockaddr*)&addrin, sizeof(addrin)) == -1)
        error("bind", 1);
    if (listen(sock, 10) == -1) //limited to 1 connection simultaazef,szoldgkg
        error("listen", 1);
    return sock;
}

int wait_for_client(int sock)
{
    struct sockaddr_in sockin;
    socklen_t socklen = sizeof(struct sockaddr_in);
    int connection_sock = accept4(sock, (sockaddr*)&sockin, &socklen, 0);//, SOCK_NONBLOCK);
    if (connection_sock == -1)
        error("accept4", 1);
    std::cout << "salut" << std::endl;
    return connection_sock;
}

bool is_request_full(std::string request)
{
    request = request;
    //fixme
    return true;
}

//fixme: possible? message splitted in multiple packets -> isolate users -> keep list of incomplete requests -> add timeout to prevent dos
//fixme: check if end of request (warning: malformed ones)
std::string get_request(int client_sock)
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
        if (is_request_full(request)) //warning: request not truncated to real size at this point (->pass size, resize may cost much)
        {
            totalsize -= 1;
            break;
        }
    }
    request.resize(totalsize + 1);
    return request;
}

void dispatch_request(int client_sock, std::string request)
{
    while (1)
    {
        for (int i = 0; i < THREADS_NB; i++)
        {
            if (!g_threads[i].is_active)
            {
                std::cout << "found inactive thread!: " << &g_threads[i] << std::endl;
                g_threads[i].client_sock = client_sock;
                g_threads[i].request = request;
                g_threads[i].is_active = true; //replace with mutex
                return;
            }
        }
    }
}

request_type get_request_type(std::string request, void*& params)
{
    request = request;
    //fixme: mutiple request types possible?
    params = new struct fileparams;
    ((struct fileparams*)params)->path = "testfile";
    return RQFILE;
}

std::string forge_error_response(error_type err)
{
    return "ERROR " + std::to_string(err) + ": ça marche pas lol \n";
}

//assuming the only request is a file retrieval
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
