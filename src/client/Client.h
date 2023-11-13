#pragma once
#include "../core/IObserver.h"
#include "../ft_irc.h"

#include <map>
#include <netdb.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Client
{
  public:
    enum RolePrefix
    {
        OPERATOR = 0,
        VOICE,
        ROLE_PREFIX_COUNT
    };

    enum UserModes
    {
        INVISIBLE = 0,
        OPER,
        USER_MODES_COUNT
    };

    Client(int fd, std::string ip, IObserver *observer);
    ~Client();

    int getFd() const;

    std::string &getSendBuffer();
    std::string &getRecvBuffer();

    void setSendBuffer(std::string const &sendBuffer);
    void setRecvBuffer(std::string const &recvBuffer);

    void send(std::string message);
    void sendMany(std::vector<std::string> message);

    RolePrefix getRoleInChannel(std::string const &channelName);
    void setRoleInChannel(std::string const &channelName, RolePrefix role);
    std::string RolePrefixToString(RolePrefix role);

    bool shouldDisconnect() const;
    void setShouldDisconnect(bool shouldDisconnect);

    bool isRegistered() const;
    void setRegistered(bool registered);

    void setPassSent(bool passSent);
    bool isPassSent() const;

    void setOp(bool op);
    bool isOp() const;

    void updtadeNickmask();

    void disconnect();

    IObserver *observer_;
    std::string nick;
    std::string username;
    std::string realname;
    std::string ip;
    std::string nickmask;

  private:
    Client(Client const &other);
    Client &operator=(Client const &rhs);
    bool operator==(Client const &rhs);
    void notifyObserver(EPOLL_EVENTS event);

    int fd_;
    bool shouldDisconnect_;
    bool registered_;

    bool passSent_;

    bool op_;
    std::map<std::string, RolePrefix> channelRoles_;

    std::string sendBuffer_;
    std::string recvBuffer_;
};
