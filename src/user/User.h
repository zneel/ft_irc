#pragma once
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class User
{
  public:
    User(int fd);
    ~User();

    int getFd() const;

    std::string &getSendBuffer();
    std::string &getRecvBuffer();

    void setNick(std::string const &nick);
    void setSendBuffer(std::string const &sendBuffer);
    void setRecvBuffer(std::string const &recvBuffer);

    void disconnect();

  private:
    User(User const &other);
    User &operator=(User const &rhs);
    bool operator==(User const &rhs);

    std::string nick_;

    int fd_;
    std::string sendBuffer_;
    std::string recvBuffer_;
};
