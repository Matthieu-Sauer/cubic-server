#include <iostream>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <exception>
#include <string.h>

#include "Server.hpp"

Server::Server(const std::string &host, uint16_t port)
    : _host(host), _port(port)
{
    std::cout << "Server created" << std::endl;
}

Server::~Server()
{
    std::cout << "Server destroyed" << std::endl;
}

int Server::launch()
{
    // Get the socket for the server
    _sockfd = socket(AF_INET, SOCK_STREAM, getprotobyname("TCP")->p_proto);

    // Create the addr for the server
    if (!inet_pton(AF_INET, _host.c_str(), &(_addr.sin_addr)))
    {
        throw std::runtime_error("Invalid host ip address");
    }
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_port);

    // Bind server socket
    int optval = 1;
    setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (bind(_sockfd, reinterpret_cast<struct sockaddr *>(&_addr), sizeof(_addr)))
    {
        throw std::runtime_error(strerror(errno));
    }

    // Listen
    listen(_sockfd, SOMAXCONN);

    auto acceptThread = std::thread(&Server::acceptLoop, this);

    gameLoop();

    return 0;
}

void Server::gameLoop()
{
    while (1)
    {
        // This is only for test purposes
        // TODO: Remove all that
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2000ms);
        std::cout << "Server ticked" << std::endl;
    }
}

void Server::acceptLoop()
{
    struct pollfd poll_set[1];

    poll_set[0].fd = _sockfd;
    poll_set[0].events = POLLIN;
    while (1)
    {
        poll(poll_set, 1, -1);
        if (poll_set[0].revents & POLLIN)
        {
            struct sockaddr_in client_addr;
            socklen_t client_addr_size = sizeof(client_addr);
            int client_fd = accept(
                _sockfd,
                reinterpret_cast<struct sockaddr *>(&client_addr),
                &client_addr_size);
            if (client_fd == -1)
            {
                throw std::runtime_error(strerror(errno));
            }
            // Add accepted client to the vector of clients
            _clients.push_back(std::make_shared<Client>(client_fd, client_addr));

            std::cout << "Client added to the list" << std::endl;
        }
    }
}
