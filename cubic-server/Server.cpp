#include <iostream>
#include <netdb.h>
#include <cerrno>
#include <sys/socket.h>
#include <poll.h>
#include <thread>
#include <chrono>
#include <exception>
#include <cstring>
#include <algorithm>

#include <nlohmann/json.hpp>

#include "Server.hpp"

#include "typeSerialization.hpp"
#include "ServerPackets.hpp"
#include "ClientPackets.hpp"

#include "Logger.hpp"

Server::Server()
    : _config()
{
    _config.parse("./config.yml");
    _host = _config.getIP();
    _port = _config.getPort();
    _maxPlayer = _config.getMaxPlayers();
    _motd = _config.getMotd();

    _log = logging::Logger::get_instance();
    _log->debug("Server created with host: " + _host + " and port: " + std::to_string(_port));
}

Server::~Server()
{
    _log->debug("Server destroyed");
}

void Server::launch()
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

//    auto acceptThread = std::thread(&Server::_acceptLoop, this);
    _acceptLoop();

//    _networkLoop();
}

void Server::_acceptLoop()
{
    struct pollfd poll_set[1];

    poll_set[0].fd = _sockfd;
    poll_set[0].events = POLLIN;
    while (true)
    {
        poll(poll_set, 1, 50);
        if (poll_set[0].revents & POLLIN)
        {
            struct sockaddr_in client_addr{};
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
            auto cli = std::make_shared<Client>(client_fd, client_addr);
            _clients.push_back(cli);
            // That line is kinda borked, but I'll check one day how to fix it
            auto *cli_thread = new std::thread(&Client::networkLoop, &(*cli));
            // That is 99.99% a data race, but aight, it will probably
            // never happen
            cli->setRunningThread(cli_thread);

//            std::cout << "Client added to the list" << std::endl;
        }

        _clients.erase(std::remove_if(
                           _clients.begin(), _clients.end(),
                           [](const std::shared_ptr<Client> &cli)
                           { return cli->isDisconnected(); }),
                       _clients.end());
    }
}
