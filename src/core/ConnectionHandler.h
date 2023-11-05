#pragma once

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
    bool sendData(int fd, std::string const &data);
    std::string recvData(int fd);

  private:
    Logger *logger_;
};
