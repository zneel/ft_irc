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
#define BUFFER_SIZE 1024

class ConnectionHandler
{
  public:
    ConnectionHandler();
    ~ConnectionHandler();
    void setLogger(Logger &logger);
    ssize_t recvData(int fd, std::string &buffer);
    ssize_t sendData(int fd, std::string &message);

  private:
    Logger *logger_;
};
