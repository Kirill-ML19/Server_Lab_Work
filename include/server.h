#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include <netinet/ip.h>
#include "implant_manager.h"

class Server {
public:
    Server(int port);
    ~Server();

    void run();
    void stop();

private:
    void accept_clients();

    int port_;
    int server_fd_;
    std::atomic<bool> running_;
    std::thread accept_thread_;
    std::vector<std::thread> client_threads_;
    ImplantManager implant_manager_;
};
