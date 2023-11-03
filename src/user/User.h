#pragma once
#include <netdb.h>
#include <sys/socket.h>

class User
{
  public:
    User(int fd);
    ~User();

    int getFd() const;

  private:
    User(User const &other);
    User &operator=(User const &rhs);
    bool operator==(User const &rhs);

    int fd_;
    struct sockaddr_storage addr_;
};
