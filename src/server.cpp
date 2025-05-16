#include "server.h"
#include "client_session.h"
#include "utils.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>

Server::Server(int port)
    : port_(port), running_(false), server_fd_(-1) {}

Server::~Server() {
    stop();
}

void Server::run() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ == -1) {
        log("Failed to create socket", LogLevel::ERROR);
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);

    int opt = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        log("setsockopt(SO_REUSEADDR) failed", LogLevel::ERROR);
        close(server_fd_);
        return;
    }

    if (bind(server_fd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
        log("Bind failed: " + std::string(std::strerror(errno)), LogLevel::ERROR);
        close(server_fd_);
        return;
    }

    if (listen(server_fd_, 10) < 0) {
        log("Listen failed: " + std::string(std::strerror(errno)), LogLevel::ERROR);
        close(server_fd_);
        return;
    }

    running_ = true;
    accept_thread_ = std::thread(&Server::accept_clients, this);

    log("Server started on port " + std::to_string(port_), LogLevel::INFO);
}

void Server::stop() {
    if (!running_)
        return;

    running_ = false;
    if (server_fd_ != -1) {
        close(server_fd_);
        server_fd_ = -1;
    }
    if (accept_thread_.joinable())
        accept_thread_.join();

    for (auto& t : client_threads_)
        if (t.joinable())
            t.join();

    log("Server stopped", LogLevel::INFO);
}

void Server::accept_clients() {
    while (running_) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd_, (sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            if (running_) {
                log("Accept failed: " + std::string(std::strerror(errno)), LogLevel::ERROR);
            }
            continue;
        }

        char client_ip[INET_ADDRSTRLEN] = {0};
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        log("New client connected: " + std::string(client_ip), LogLevel::INFO);

        client_threads_.emplace_back([client_fd, this]() {
            ClientSession session(client_fd, implant_manager_);
            session.run();
            shutdown(client_fd, SHUT_RDWR);
            close(client_fd);
        });
    }
}
