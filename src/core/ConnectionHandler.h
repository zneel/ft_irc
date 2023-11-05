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
    ConnectionHandler();
    ~ConnectionHandler();
    void setLogger(Logger &logger);
    bool recvData(int fd, std::string &buffer);
    bool sendData(int fd, std::string &message);

  private:
    Logger *logger_;
};
