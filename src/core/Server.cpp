#include "Server.h"
#include <algorithm>
#include <iterator>

Server::Server(std::string port, std::string password) : port_(port), password_(password), run_(true), listener_(-1)
{
}

Server::~Server()
{
}

void Server::start()
{
    if ((listener_ = listenForConn()) < 0)
        errorExit("cannot get listening socket");

    addToPollFds(listener_);
    std::cout << "ircserver listening on port " << port_ << std::endl;
    while (run_)
    {
        if (poll(pfds_.data(), pfds_.size(), -1) <= 0)
            errorExit("poll");
        for (size_t i = 0; i < pfds_.size(); ++i)
        {
            if (pfds_[i].revents & POLLIN)
            {
                if (pfds_[i].fd == listener_)
                {
                    struct sockaddr_storage remoteAddr;
                    socklen_t addrlen = sizeof(remoteAddr);
                    int newFd;
                    if ((newFd = accept(listener_, (struct sockaddr *)&remoteAddr, &addrlen)) < 0)
                        std::cerr << "error: accept" << std::endl;
                    else
                    {
                        addToPollFds(newFd);
                        uManager_.create(newFd);
                        fcntl(newFd, F_SETFL, O_NONBLOCK);
                        char *remoteIp = inet_ntoa(*(struct in_addr *)getInAddr((struct sockaddr *)&remoteAddr));
                        std::cout << "new connection from ip: " << remoteIp << std::endl;
                    }
                }
                else
                {
                    std::fill_n(buff_, sizeof(buff_), 0);
                    int nbytes = recv(pfds_[i].fd, buff_, sizeof(buff_), 0);
                    if (nbytes <= 0)
                    {
                        if (nbytes == 0)
                            std::cerr << "connection closed" << std::endl;
                        else
                            std::cerr << "error: recv" << std::endl;
                        close(pfds_[i].fd);
                        std::vector<pollfd>::iterator it = pfds_.begin() + i;
                        pfds_.erase(it);
                    }
                    else
                    {
                        for (std::vector<pollfd>::iterator it1 = pfds_.begin(); it1 != pfds_.end(); ++it1)
                        {
                            if (it1->fd != listener_ && it1->fd != pfds_[i].fd)
                            {
                                if (send(it1->fd, buff_, nbytes, 0) == -1)
                                    std::cerr << "error: send" << std::endl;
                            }
                        }
                    }
                }
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

void Server::errorExit(std::string const &msg) const
{
    std::cerr << "error: " << msg << std::endl;
    Server::~Server();
    exit(1);
}

void Server::addToPollFds(int fd)
{
    struct pollfd pfd;
    pfd.events = POLLIN;
    pfd.fd = fd;
    pfds_.push_back(pfd);
}

void *Server::getInAddr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in *)sa)->sin_addr);
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int Server::listenForConn()
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
        std::cerr << "error: getaddrinfo" << std::endl;
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
    if (listen(sockfd, 10) < 0)
        return -1;
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    return sockfd;
}
