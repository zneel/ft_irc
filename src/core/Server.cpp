#include "Server.h"
#include <fcntl.h>
#include <unistd.h>

Server::Server(std::string port, std::string password) : port_(port), password_(password), listener_(-1)
{
    connectionHandler_.setLogger(logger_);
}

Server::~Server()
{
    uManager_.removeAll();
    for (PollFdsIterator it = pollFds_.begin(); it != pollFds_.end(); ++it)
        close(it->fd);
    pollFds_.clear();
}

void Server::start()
{
    if ((listener_ = listenForConnection()) < 0)
        throw std::runtime_error("listenForConnection");

    initListener();
    logger_.log("ircserv listening on port " + port_, Logger::INFO);
    while (!stop)
    {
        int pollCount = poll(pollFds_.data(), pollFds_.size(), 100);
        if (pollCount < 0)
        {
            logger_.log("poll: " + std::string(strerror(errno)), Logger::ERROR);
            break;
        }
        for (size_t i = 0; i < pollFds_.size(); i++)
        {
            if (pollFds_[i].revents & POLLIN)
            {
                if (pollFds_[i].fd == listener_)
                    acceptConnection();
                else
                {
                    std::string data = connectionHandler_.recvData(pollFds_[i].fd);
                    std::cout << "size: " << data.size() << std::endl;
                    std::cout << "data: " << data << std::endl;
                    if (data.empty())
                    {
                        removeFromPolling(pollFds_[i].fd, i);
                        continue;
                    }
                    for (size_t j = 0; j < pollFds_.size(); j++)
                    {
                        if (pollFds_[j].fd == listener_)
                            continue;
                        if (pollFds_[i].fd == pollFds_[j].fd)
                            continue;
                        connectionHandler_.sendData(pollFds_[j].fd, data);
                    }
                }
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
    int newFd = accept(listener_, (struct sockaddr *)&remAddr, &addrlen);
    if (newFd < 0)
        logger_.log("accept", Logger::ERROR);
    else
    {
        connectAndAddToPolling(newFd);
        uManager_.create(newFd);
        char remoteIp[INET6_ADDRSTRLEN];
        // @TODO REWRITE inet_ntop
        inet_ntop(remAddr.ss_family, getInAddr_((struct sockaddr *)&remAddr), remoteIp, INET6_ADDRSTRLEN);
        logger_.log("new connection from " + std::string(remoteIp), Logger::INFO);
    }
}

void Server::connectAndAddToPolling(int fd)
{
    struct pollfd pfd;
    pfd.events = POLLIN | POLLOUT;
    pfd.fd = fd;
    pfd.revents = 0;
    pollFds_.push_back(pfd);
}

void Server::removeFromPolling(int fd, int i)
{
    uManager_.remove(fd);
    PollFdsIterator pollIt = pollFds_.begin() + i;
    pollFds_.erase(pollIt);
}

void Server::initListener()
{
    struct pollfd pfd;
    pfd.events = POLLIN;
    pfd.fd = listener_;
    pfd.revents = 0;
    pollFds_.push_back(pfd);
}

int Server::listenForConnection()
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
    return sockfd;
}
