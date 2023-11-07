#include "User.h"

User::User(int fd) : fd_(fd)
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

void User::setNick(std::string const &nick) {
  nick_ = nick;
}

void User::setSendBuffer(std::string const &sendBuffer)
{
    sendBuffer_ = sendBuffer;
}

void User::setRecvBuffer(std::string const &recvBuffer)
{
    recvBuffer_ = recvBuffer;
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
