#include "Channel.h"

Channel::Channel(std::string name, int mode, int type) : name_(name), mode_(mode), type_(type)
{
}

Channel::~Channel()
{
}

std::map<int, User *> Channel::getUsers()
{
    return users_;
}

std::map<int, User *> Channel::getOperators()
{
    return operators_;
}

std::string Channel::getName() const
{
    return name_;
}

void Channel::setName(std::string name)
{
    name_ = name;
}

int Channel::getMode()
{
    return mode_;
}

void Channel::setMode(int mode)
{
    mode_ = mode;
}

void Channel::addMode(int mode)
{
    mode_ |= mode;
}

bool Channel::hasMode(int mode)
{
    return mode_ & mode;
}

int Channel::getType()
{
    return type_;
}

void Channel::setType(int type)
{
    type_ = type;
}

void Channel::addType(int type)
{
    type_ |= type;
}

bool Channel::hasType(int type)
{
    return type_ & type;
}

void Channel::addUser(User *user)
{
    users_[user->getFd()] = user;
}

void Channel::removeUser(User *user)
{
    users_.erase(user->getFd());
}

void Channel::addOperator(User *user)
{
    operators_[user->getFd()] = user;
}

void Channel::removeOperator(User *user)
{
    operators_.erase(user->getFd());
}

int Channel::getUserCount()
{
    return users_.size();
}
