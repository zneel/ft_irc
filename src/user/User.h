#pragma once
#include <netdb.h>
#include <string>
#include <sys/socket.h>

class User
{
  public:
    User(int fd);
    ~User();

    int getFd() const;
    std::string &getMessage();

  private:
    User(User const &other);
    User &operator=(User const &rhs);
    bool operator==(User const &rhs);

    int fd_;
    std::string message_;
    struct sockaddr_storage addr_;
};
