#pragma once

#include "../buffer/Buffer.h"
#include "../channel/ChannelManager.h"
#include "../commands/CommandManager.h"
#include "../ft_irc.h"
#include "../user/User.h"
#include "../user/UserManager.h"
#include "Logger.h"

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <iterator>
#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define MAX_LISTENER 64
#define MAX_EVENTS 512

class Server
{

    typedef std::vector<epoll_event>::iterator EventsIterator;

  public:
    Server(std::string port, std::string password);
    ~Server();

    void start();
    void setLogger(Logger &logger);

  private:
    Server(Server const &other);
    Server &operator=(Server const &rhs);

    void *getInAddr_(struct sockaddr *sa);
    void initListener();

    int listenForConnection();
    void acceptConnection();

    void removeFromPolling(int fd);
    void addToPolling(int fd);

    void sendData(struct epoll_event &event);
    void recvData(struct epoll_event &event, CommandManager &commands);

    bool hasCRLF(std::string &buffer);

    void disconnectUsers();

    std::string const port_;
    std::string const password_;

    Logger logger_;

    UserManager uManager_;
    ChannelManager cManager_;

    int epollfd_;
    std::vector<epoll_event> events_;

    int listener_;
};
