#include "User.h"

User::User(int fd) : fd_(fd)
{
}

User::~User()
{
}

int User::getFd() const
{
    return fd_;
}

std::string &User::getMessage()
{
    return message_;
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
