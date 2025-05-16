#pragma once

#include <netinet/in.h>
#include <string>
#include "implant_manager.h"

class ClientSession {
public:
    ClientSession(int client_fd, ImplantManager& implant_manager);
    void run();

private:
    int client_fd_;
    ImplantManager& implant_manager_;

    void handle_client();
};
