#include "Server.h"
#include <iostream>

Server::Server(int port, std::string password) : port_(port), password_(password)
{
    std::cout << port << password << std::endl;
}

Server::~Server()
{
}

Server::Server(Server const &other) : port_(other.port_), password_(other.password_)
{
}

Server &Server::operator=(Server const &rhs)
{
    if (this != &rhs)
    {
        port_ = rhs.port_;
        password_ = rhs.password_;
    }
    return *this;
}
