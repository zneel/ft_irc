#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "../ft_irc.h"
#include "../user/User.h"
#include "../user/UserManager.h"
#include "ConnectionHandler.h"
#include "Logger.h"

#define MAX_LISTENER 10

class Server
{

    typedef std::vector<pollfd>::iterator PollFdsIterator;

  public:
    Server(std::string port, std::string password);
    ~Server();

    void start();
    void setLogger(Logger &logger);

  private:
    void addToPolling(int fd);
    void *getInAddr_(struct sockaddr *sa);
    int listenForConnection();
    void acceptConnection();
    void handleClient(struct pollfd pfd, int i);
    void removeFromPolling(int fd, int i);
    void initListener();

    std::string const port_;
    std::string const password_;
    Logger logger_;

    UserManager uManager_;

    std::vector<pollfd> pollFds_;
    std::vector<ConnectionHandler::ClientBuffer> clientBuffers_;

    ConnectionHandler connectionHandler_;

    int listener_;
};
