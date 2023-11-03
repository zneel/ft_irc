#pragma once

#include <string>

class Server
{
  public:
    Server(int port, std::string password);
    ~Server();

  private:
    Server(Server const &other);
    Server &operator=(Server const &rhs);

    int port_;
    std::string password_;
};
