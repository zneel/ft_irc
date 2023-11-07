#include "Server.h"
#include "../buffer/Buffer.h"
#include "ConnectionHandler.h"
#include <asm-generic/socket.h>
#include <stdexcept>
#include <sys/epoll.h>
#include <sys/types.h>

Server::Server(std::string port, std::string password) : port_(port), password_(password), listener_(-1)
{
    handler_.setLogger(logger_);
    welcomeMessage_.append(LINE1) + "\r\n";
    welcomeMessage_.append(LINE2) + "\r\n";
    welcomeMessage_.append(LINE3) + "\r\n";
    welcomeMessage_.append(LINE4) + "\r\n";
    welcomeMessage_.append(LINE5) + "\r\n";
    welcomeMessage_.append(LINE6) + "\r\n";
    welcomeMessage_.append(LINE7) + "\r\n";
    welcomeMessage_.append(LINE8) + "\r\n";
    welcomeMessage_.append(LINE9) + "\r\n";
    welcomeMessage_.append(LINE10) + "\r\n";
    welcomeMessage_.append(LINE11) + "\r\n";
}

Server::~Server()
{
    uManager_.removeAll();
    for (EventsIterator it = events_.begin(); it != events_.end(); ++it)
        epoll_ctl(epollfd_, EPOLL_CTL_DEL, it->data.fd, NULL);
    events_.clear();
    close(listener_);
    close(epollfd_);
}

void Server::start()
{
    epollfd_ = epoll_create1(0);
    if (epollfd_ < 0)
        throw std::runtime_error("epoll_create1: " + std::string(strerror(errno)));
    if ((listener_ = listenForConnection()) < 0)
        throw std::runtime_error("listenForConnection");
    CommandManager commands(&cManager_, &uManager_);
    initListener();
    logger_.log("ircserv listening on port " + port_, Logger::INFO);
    while (!stop)
    {
        ssize_t eventCount = epoll_wait(epollfd_, events_.data(), events_.size(), -1);
        if (eventCount < 0)
            throw std::runtime_error("epoll_wait: " + std::string(strerror(errno)));
        for (int i = 0; i < eventCount; ++i)
            std::cout << "event: " << events_[i].data.fd << std::endl;
        for (ssize_t i = 0; i < eventCount; ++i)
        {
            if (events_[i].events & EPOLLHUP)
            {
                uManager_.remove(events_[i].data.fd);
                removeFromPolling(events_[i].data.fd, i);
                continue;
            }
            if (events_[i].events & EPOLLIN)
            {
                if (events_[i].data.fd == listener_)
                    acceptConnection();
                else
                    recvData(events_[i], commands, i);
            }
            if (events_[i].events & EPOLLOUT)
                sendData(events_[i]);
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
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        addToPolling(fd);
        uManager_.create(fd);
        char remoteIp[INET6_ADDRSTRLEN];
        // @TODO REWRITE inet_ntop
        inet_ntop(remAddr.ss_family, getInAddr_((struct sockaddr *)&remAddr), remoteIp, INET6_ADDRSTRLEN);
        logger_.log("new connection from " + std::string(remoteIp), Logger::INFO);
    }
}

void Server::addToPolling(int fd)
{
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = EPOLLIN;
    event.data.fd = fd;
    events_.push_back(event);
    if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event) < 0)
        throw std::runtime_error("epoll_ctl: " + std::string(strerror(errno)));
}

void Server::sendData(struct epoll_event &event)
{
    if (event.data.fd == listener_)
        return;
    ssize_t bytesSent = handler_.sendData(event.data.fd, uManager_.get(event.data.fd)->getSendBuffer());
    ssize_t len = uManager_.get(event.data.fd)->getSendBuffer().size();
    std::cout << "sent message: " << uManager_.get(event.data.fd)->getSendBuffer() << std::endl;
    if (len - bytesSent == 0)
    {
        uManager_.get(event.data.fd)->getSendBuffer().clear();
        event.events = EPOLLIN;
    }
    else
        uManager_.get(event.data.fd)->getSendBuffer().erase(0, bytesSent);
}

void Server::recvData(struct epoll_event &event, CommandManager &commands, int i)
{
    ssize_t received = handler_.recvData(event.data.fd, uManager_.get(event.data.fd)->getRecvBuffer());
    if (received == 0)
    {

        removeFromPolling(event.data.fd, i);
        uManager_.remove(event.data.fd);
        return;
    }
    std::cout << "received buffer: " << uManager_.get(event.data.fd)->getRecvBuffer() << std::endl;
    if (received > 0)
    {
        if (hasCRLF(uManager_.get(event.data.fd)->getRecvBuffer()))
        {
            std::deque<Message> msgs;
            Buffer::bufferToMessage(uManager_.get(event.data.fd)->getRecvBuffer(), msgs);
            commands.doCommands(msgs, uManager_.get(event.data.fd));
            event.events = EPOLLOUT;
        }
    }
}

void Server::removeFromPolling(int fd, int i)
{
    logger_.log("connection closed", Logger::INFO);
    if (epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, NULL) < 0)
        throw std::runtime_error("epoll_ctl: " + std::string(strerror(errno)));
    events_.erase(events_.begin() + i);
}

void Server::initListener()
{
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = EPOLLIN | EPOLLPRI;
    event.data.fd = listener_;
    events_.push_back(event);
    if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, listener_, &event) < 0)
        throw std::runtime_error("epoll_ctl: " + std::string(strerror(errno)));
}

bool Server::hasCRLF(std::string &buffer)
{
    return (buffer.find("\r\n") != std::string::npos);
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
        throw std::runtime_error("getaddrinfo: " + std::string(strerror(errno)));
    for (curr = addr; curr; curr = curr->ai_next)
    {
        if ((sockfd = socket(curr->ai_family, curr->ai_socktype, curr->ai_protocol)) < 0)
            continue;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof(int));
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
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    return sockfd;
}
