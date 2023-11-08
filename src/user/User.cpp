#include "User.h"

User::User(int fd, std::string ip)
    : nick(""), username(""), realname(""), isupportTokenPackCount(0), ip(ip), fd_(fd), shouldDisconnect_(false),
      registered_(false), passSent_(false)
{
}

User::~User()
{
    disconnect();
}

int User::getFd() const
{
    return fd_;
}

std::string &User::getSendBuffer()
{
    return sendBuffer_;
}

std::string &User::getRecvBuffer()
{
    return recvBuffer_;
}

void User::setSendBuffer(std::string const &sendBuffer)
{
    sendBuffer_ = sendBuffer;
}

void User::setRecvBuffer(std::string const &recvBuffer)
{
    recvBuffer_ = recvBuffer;
}

void User::send(std::string message)
{
    if (message.empty())
        return;
    if (message.length() > MAX_SIZE_SEND_BUFFER)
        message.erase(MAX_SIZE_SEND_BUFFER, std::string::npos);
    sendBuffer_.append(message + CRLF);
}

bool User::shouldDisconnect() const
{
    return shouldDisconnect_ == true;
}

void User::setShouldDisconnect(bool shouldDisconnect)
{
    shouldDisconnect_ = shouldDisconnect;
}

bool User::isRegistered() const
{
    return registered_ == true;
}

void User::setRegistered(bool registered)
{
    registered_ = registered;
}

void User::setPassSent(bool passSent)
{
    passSent_ = passSent;
}

bool User::isPassSent() const
{
    return passSent_ == true;
}

void User::disconnect()
{
    close(fd_);
    fd_ = -1;
}

User::User(User const &other) : fd_(other.fd_)
{
}

User &User::operator=(User const &rhs)
{
    if (this != &rhs)
    {
    }
    return *this;
}

bool User::operator==(User const &rhs)
{
    return fd_ == rhs.fd_;
}
