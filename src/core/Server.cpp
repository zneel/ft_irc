#include "Server.h"
#include "../buffer/Buffer.h"
#include "ConnectionHandler.h"
#include <fcntl.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>

Server::Server(std::string port, std::string password) : port_(port), password_(password), listener_(-1)
{
    handler_.setLogger(logger_);
}

Server::~Server()
{
    uManager_.removeAll();
    for (PollFdsIterator it = poller_.begin(); it != poller_.end(); ++it)
        close(it->fd);
    poller_.clear();
}

void Server::start()
{
    if ((listener_ = listenForConnection()) < 0)
        throw std::runtime_error("listenForConnection");

    initListener();
    logger_.log("ircserv listening on port " + port_, Logger::INFO);
    while (!stop)
    {
        poll(poller_.data(), poller_.size(), -1);
        for (size_t i = 0; i < poller_.size(); i++)
        {
            if (poller_[i].revents & POLLIN)
            {
                if (poller_[i].fd == listener_)
                    acceptConnection();
                else
                {
                    ssize_t received = handler_.recvData(poller_[i].fd, uManager_.get(poller_[i].fd)->getRecvBuffer());
                    if (received == 0)
                    {
                        removeFromPolling(poller_[i].fd, i);
                        continue;
                    }
                    std::cout << "received buffer: " << uManager_.get(poller_[i].fd)->getRecvBuffer() << std::endl;
                    if (received > 0)
                    {
                        if (hasCRLF(uManager_.get(poller_[i].fd)->getRecvBuffer()))
                        {
                            Buffer::treatBuffer(uManager_.get(poller_[i].fd)->getRecvBuffer(),
                                                uManager_.get(poller_[i].fd)->getSendBuffer());
                            poller_[i].events |= POLLOUT;
                        }
                    }
                }
            }
            if (poller_[i].revents & POLLOUT)
            {
                if (poller_[i].fd == listener_)
                    continue;
                ssize_t bytesSent = 1;
                ssize_t len = uManager_.get(poller_[i].fd)->getSendBuffer().size();
                std::cout << "sent message: " << uManager_.get(poller_[i].fd)->getSendBuffer() << std::endl;
                if (len - bytesSent == 0)
                {
                    uManager_.get(poller_[i].fd)->getSendBuffer().clear();
                    poller_[i].events = POLLIN;
                }
                else
                    uManager_.get(poller_[i].fd)->getSendBuffer().erase(0, bytesSent);
            }
        }
    }
}

void Server::setLogger(Logger &logger)
{
    logger_ = logger;
}

void *Server::getInAddr_(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in *)sa)->sin_addr);
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void Server::acceptConnection()
{
    struct sockaddr_storage remAddr;
    socklen_t addrlen = sizeof(remAddr);
    int fd = accept(listener_, (struct sockaddr *)&remAddr, &addrlen);
    if (fd < 0)
        logger_.log("accept", Logger::ERROR);
    else
    {
        fcntl(fd, F_SETFL, O_NONBLOCK);
        addToPolling(fd);
        /*User *u = */ uManager_.create(fd);
        char remoteIp[INET6_ADDRSTRLEN];
        // @TODO REWRITE inet_ntop
        inet_ntop(remAddr.ss_family, getInAddr_((struct sockaddr *)&remAddr), remoteIp, INET6_ADDRSTRLEN);
        logger_.log("new connection from " + std::string(remoteIp), Logger::INFO);
    }
}

void Server::addToPolling(int fd)
{
    struct pollfd pfd;
    pfd.events = POLLIN;
    pfd.fd = fd;
    pfd.revents = 0;
    poller_.push_back(pfd);
}

void Server::removeFromPolling(int fd, int i)
{
    uManager_.remove(fd);
    PollFdsIterator pollIt = poller_.begin() + i;
    poller_.erase(pollIt);
}

void Server::initListener()
{
    struct pollfd pfd;
    pfd.events = POLLIN;
    pfd.fd = listener_;
    pfd.revents = 0;
    poller_.push_back(pfd);
}

bool Server::hasCRLF(std::string &buffer)
{
    return (buffer.find("\r\n") != std::string::npos);
}

bool Server::hasLF(std::string &buffer)
{
    return (buffer.find("\n") != std::string::npos);
}

int Server::listenForConnection()
{
    struct addrinfo hints, *addr, *curr;
    int status;
    int sockfd;
    int yes = 1;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((status = getaddrinfo(NULL, port_.c_str(), &hints, &addr)) != 0)
    {
        logger_.log("getaddrinfo: " + std::string(strerror(errno)), Logger::ERROR);
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
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    return sockfd;
}
