#include "header.hh"

std::vector<struct threadinfo> g_threads;
void* thread_routine(void* args);

void init_threads()
{
    g_threads.resize(THREADS_NB);
    for (int i = 0; i < THREADS_NB; i++)
    {
        if (pthread_create(&g_threads[i].th, NULL, thread_routine, &g_threads[i]))
            error("pthread_create", 1);
        g_threads[i].is_active = false;
    }
}

void* thread_routine(void* args)
{
    struct threadinfo& self = *(struct threadinfo*)args;
    std::cout << &self << std::endl;
    //fixme: replace with mutex
    while (1)
    {
        while (!self.is_active)
        {
        }
        std::cout << "c partit ma grosse bite!" << std::endl;
        //start of analyse
        void* params = NULL;
        request_type rqtype = get_request_type(self.request, params);
        std::string response = forge_response(rqtype, params);

        std::cout << "response is" << std::endl << response << "end of response" << std::endl;
        std::cout << "sending response:..." << std::endl;

        send_reponse(self.client_sock, response);

        std::cout << "done" << std::endl;
        close(self.client_sock); //fixme: should we close this
        //end of analyse

        self.is_active = false;
    }
}
