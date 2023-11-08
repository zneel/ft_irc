#pragma once
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class User
{
  public:
    User(int fd, std::string ip);
    ~User();

    int getFd() const;

    std::string &getSendBuffer();
    std::string &getRecvBuffer();

    void setSendBuffer(std::string const &sendBuffer);
    void setRecvBuffer(std::string const &recvBuffer);

    bool shouldDisconnect() const;
    void setShouldDisconnect(bool shouldDisconnect);

    bool isRegistered() const;
    void setRegistered(bool registered);

    void setPassSent(bool passSent);
    bool isPassSent() const;

    void disconnect();

    std::string nick;
    std::string username;
    std::string realname;
    std::string ip;

  private:
    User(User const &other);
    User &operator=(User const &rhs);
    bool operator==(User const &rhs);

    int fd_;

    bool shouldDisconnect_;
    bool registered_;

    bool passSent_;

    std::string sendBuffer_;
    std::string recvBuffer_;
};
