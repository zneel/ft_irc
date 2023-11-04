#include "Server.h"

#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>

Server::Server(std::string port, std::string password) : port_(port), password_(password), run_(true), listener_(-1)
{
}

Server::~Server()
{
}

void Server::start()
{
    if ((listener_ = listenForConn_()) < 0)
        errorExit_("cannot get listening socket");

    addClient_(listener_);
    std::cout << "ircserver: listening on port: " << port_ << std::endl;
    while (run_)
    {
        if (poll(clients_.data(), clients_.size(), -1) <= 0)
            errorExit_("poll");

        for (size_t i = 0; i < clients_.size(); ++i)
        {
            if (clients_[i].revents & POLLIN)
            {
                if (clients_[i].fd == listener_)
                    acceptConn_();
                else
                    handleClient_(clients_[i], i);
            }
        }
    }
}

void Server::stop()
{
    run_ = false;
}

Server::Server(Server const &other) : port_(other.port_), password_(other.password_)
{
}

Server &Server::operator=(Server const &rhs)
{
    if (this != &rhs)
    {
    }
    return *this;
}

void Server::errorExit_(std::string const &msg) const
{
    std::cerr << "error: " << msg << std::endl;
    Server::~Server();
    exit(1);
}

void Server::addClient_(int fd)
{
    struct pollfd pfd;
    pfd.events = POLLIN;
    pfd.fd = fd;
    pfd.revents = 0;
    clients_.push_back(pfd);
}

void *Server::getInAddr_(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in *)sa)->sin_addr);
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int Server::listenForConn_()
{
    struct addrinfo hints, *addr, *curr;
    int status;
    int sockfd;
    int yes = 1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((status = getaddrinfo(NULL, port_.c_str(), &hints, &addr)) != 0)
    {
        std::cerr << "error: getaddrinfo: " << strerror(errno) << std::endl;
        exit(1);
    }
    for (curr = addr; curr; curr = curr->ai_next)
    {
        if ((sockfd = socket(curr->ai_family, curr->ai_socktype, curr->ai_protocol)) < 0)
            continue;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (bind(sockfd, curr->ai_addr, curr->ai_addrlen) < 0)
        {
            close(sockfd);
            continue;
        }
        break;
    }
    freeaddrinfo(addr);
    if (!curr)
        return -1;

    if (listen(sockfd, MAX_LISTENER) < 0)
        return -1;

    return sockfd;
}

void Server::acceptConn_()
{
    struct sockaddr_storage remAddr;
    socklen_t addrlen = sizeof(remAddr);
    int newFd = accept(listener_, (struct sockaddr *)&remAddr, &addrlen);
    if (newFd < 0)
        std::cerr << "error: accept: " << strerror(errno) << std::endl;
    else
    {
        addClient_(newFd);
        uManager_.create(newFd);
        char remoteIp[INET6_ADDRSTRLEN];
        // @TODO REWRITE inet_ntop
        // @TODO REWRITE inet_ntop
        // @TODO REWRITE inet_ntop
        // @TODO REWRITE inet_ntop
        inet_ntop(remAddr.ss_family, getInAddr_((struct sockaddr *)&remAddr), remoteIp,
                  INET6_ADDRSTRLEN); // @TODO REWRITE inet_ntop
        // inet_ntoa(*(struct in_addr *)getInAddr_((struct sockaddr *)&remAddr));
        std::cout << "new connection from ip: " << remoteIp << std::endl;
    }
}

void Server::handleClient_(struct pollfd pfd, int i)
{
    std::fill_n(buff_, sizeof(buff_), 0);
    int nbytes = recv(pfd.fd, buff_, sizeof(buff_), 0);
    if (nbytes <= 0)
    {
        if (nbytes == 0)
            std::cerr << "connection closed" << std::endl;
        else
            std::cerr << "error: recv: " << strerror(errno) << std::endl;
        disconnectClient_(pfd.fd, i);
    }
    else
    {
        for (PfdIterator it1 = clients_.begin(); it1 != clients_.end(); ++it1)
        {
            // dispatch messages to all clients but the sender
            std::cout << "message: " << buff_ << std::endl;
        }
    }
}

void Server::disconnectClient_(int fd, int i)
{
    uManager_.remove(fd);
    close(fd);
    PfdIterator it = clients_.begin() + i;
    clients_.erase(it);
}
