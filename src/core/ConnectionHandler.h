#pragma once

#include "../ft_irc.h"
#include "Logger.h"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

class ConnectionHandler
{

  public:
    struct ClientBuffer
    {
        std::vector<char> buffer;
        size_t bytesSent;
    };
    ConnectionHandler();
    ~ConnectionHandler();
    void setLogger(Logger &logger);
    bool sendData(int fd, ClientBuffer &buffer);
    bool recvData(int fd, ClientBuffer &buffer);

  private:
    Logger *logger_;
};
