#include "Server.h"
#include "../buffer/Buffer.h"
#include "ConnectionHandler.h"

Server::Server(std::string port, std::string password) : port_(port), password_(password), listener_(-1)
{
    events_.reserve(MAX_EVENTS);
    handler_.setLogger(logger_);
    welcomeMessage_.append(LINE1) + CRLF;
    welcomeMessage_.append(LINE2) + CRLF;
    welcomeMessage_.append(LINE3) + CRLF;
    welcomeMessage_.append(LINE4) + CRLF;
    welcomeMessage_.append(LINE5) + CRLF;
    welcomeMessage_.append(LINE6) + CRLF;
    welcomeMessage_.append(LINE7) + CRLF;
    welcomeMessage_.append(LINE8) + CRLF;
    welcomeMessage_.append(LINE9) + CRLF;
    welcomeMessage_.append(LINE10) + CRLF;
    welcomeMessage_.append(LINE11) + CRLF;
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
    CommandManager commands(&cManager_, &uManager_, password_);
    initListener();
    logger_.log("ircserv listening on port " + port_, Logger::INFO);
    while (!stop)
    {
        disconnectUsers();
        ssize_t eventCount = epoll_wait(epollfd_, events_.data(), events_.size(), -1);
        if (eventCount < 0)
            throw std::runtime_error("epoll_wait: " + std::string(strerror(errno)));
        for (ssize_t i = 0; i < eventCount; ++i)
        {
            if (events_[i].events & EPOLLHUP)
            {
                removeFromPolling(events_[i].data.fd);
                uManager_.remove(events_[i].data.fd);
                break;
            }
            else if (events_[i].events & EPOLLIN)
            {
                if (events_[i].data.fd == listener_)
                    acceptConnection();
                else
                {
                    if (uManager_.get(events_[i].data.fd) && uManager_.get(events_[i].data.fd)->shouldDisconnect())
                        continue;
                    recvData(events_[i], commands);
                }
            }
            else if (events_[i].events & EPOLLOUT)
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
        return &reinterpret_cast<struct sockaddr_in *>(sa)->sin_addr;
    return &reinterpret_cast<struct sockaddr_in6 *>(sa)->sin6_addr;
}

void Server::acceptConnection()
{
    struct sockaddr_storage remAddr;
    socklen_t addrlen = sizeof(remAddr);
    int fd = accept(listener_, (struct sockaddr *)&remAddr, &addrlen);
    if (fd < 0)
        logger_.log(std::string("accept: ") + strerror(errno), Logger::ERROR);
    else
    {
        char remoteIp[INET6_ADDRSTRLEN];
        // @TODO REWRITE inet_ntop
        inet_ntop(remAddr.ss_family, getInAddr_((struct sockaddr *)&remAddr), remoteIp, INET6_ADDRSTRLEN);
        fcntl(fd, F_SETFL, O_NONBLOCK);
        addToPolling(fd);
        uManager_.create(fd, remoteIp);
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
        logger_.log(std::string("epoll_ctl: ") + strerror(errno), Logger::ERROR);
}

void Server::removeFromPolling(int fd)
{
    logger_.log("connection closed", Logger::INFO);
    if (epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, NULL) < 0)
        logger_.log(std::string("epoll_ctl: ") + strerror(errno), Logger::ERROR);
    for (EventsIterator it = events_.begin(); it != events_.end(); ++it)
    {
        if (it->data.fd == fd)
        {
            events_.erase(it);
            break;
        }
    }
}

void Server::sendData(struct epoll_event &event)
{
    if (!uManager_.get(event.data.fd))
        return;
    ssize_t bytesSent = handler_.sendData(event.data.fd, uManager_.get(event.data.fd)->getSendBuffer());
    ssize_t len = uManager_.get(event.data.fd)->getSendBuffer().size();
    if (len - bytesSent == 0)
    {
        std::string s = uManager_.get(event.data.fd)->getSendBuffer();
        for (size_t pos = 0; (pos = s.find("\r\n", pos)) != std::string::npos; s.replace(pos, 2, "\\r\\n"), pos += 2)
            ;
        logger_.log(">" + s, Logger::INFO);
        uManager_.get(event.data.fd)->getSendBuffer().clear();
        event.events = EPOLLIN;
        epoll_ctl(epollfd_, EPOLL_CTL_MOD, event.data.fd, &event);
    }
    else
        uManager_.get(event.data.fd)->getSendBuffer().erase(0, bytesSent);
}

void Server::recvData(struct epoll_event &event, CommandManager &commands)
{
    if (!uManager_.get(event.data.fd))
        return;
    ssize_t received = handler_.recvData(event.data.fd, uManager_.get(event.data.fd)->getRecvBuffer());
    if (received == 0)
    {
        removeFromPolling(event.data.fd);
        uManager_.remove(event.data.fd);
        return;
    }
    if (received > 0)
    {
        std::cout << "received buffer: " << uManager_.get(event.data.fd)->getRecvBuffer() << std::endl;
        if (hasCRLF(uManager_.get(event.data.fd)->getRecvBuffer()))
        {
            std::deque<Message> msgs;
            Buffer::bufferToMessage(uManager_.get(event.data.fd)->getRecvBuffer(), msgs);
            commands.doCommands(msgs, uManager_.get(event.data.fd));
            event.events = EPOLLOUT;
            epoll_ctl(epollfd_, EPOLL_CTL_MOD, event.data.fd, &event);
        }
    }
}

void Server::initListener()
{
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = EPOLLIN | EPOLLPRI;
    event.data.fd = listener_;
    events_.push_back(event);
    if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, listener_, &event) < 0)
        logger_.log(std::string("epoll_ctl: ") + strerror(errno), Logger::ERROR);
}

bool Server::hasCRLF(std::string &buffer)
{
    return (buffer.find("\r\n") != std::string::npos);
}

void Server::disconnectUsers()
{
    std::vector<int> toRemove;
    for (std::map<int, User *>::iterator it = uManager_.getUsers().begin(); it != uManager_.getUsers().end(); ++it)
    {
        if (it->second->shouldDisconnect() && it->second->getSendBuffer().empty())
            toRemove.push_back(it->first);
    }
    for (std::vector<int>::iterator it = toRemove.begin(); it != toRemove.end(); ++it)
    {
        removeFromPolling(*it);
        uManager_.remove(*it);
    }
}

int Server::listenForConnection()
{
    struct addrinfo hints, *addr, *curr;
    int status;
    int sockfd;
    int yes = 1;
    int no = 0;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((status = getaddrinfo(NULL, port_.c_str(), &hints, &addr)) != 0)
        throw std::runtime_error("getaddrinfo: " + std::string(strerror(errno)));
    for (curr = addr; curr; curr = curr->ai_next)
    {
        if ((sockfd = socket(curr->ai_family, curr->ai_socktype, curr->ai_protocol)) < 0)
            continue;

        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
        setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &no, sizeof(no));

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
