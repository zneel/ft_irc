#include "Channel.h"

Channel::Channel(std::string name, Mode mode, Type type) : name(name), password(""), topic(""), mode_(mode), type_(type)
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

bool Channel::hasModes(int modes)
{
    return (mode_ & modes) == modes;
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

std::string Channel::modeToString()
{
    std::string mode;
    if (hasMode(Channel::BAN))
        mode += "+b";
    if (hasMode(Channel::EXCEPTION))
        mode += "+e";
    if (hasMode(Channel::CLIENT_LIMIT))
        mode += "+l";
    if (hasMode(Channel::INVITE_ONLY))
        mode += "+i";
    if (hasMode(Channel::INVITE_EXCEPTION))
        mode += "+I";
    if (hasMode(Channel::KEY))
        mode += "+k";
    if (hasMode(Channel::MODERATED))
        mode += "+m";
    if (hasMode(Channel::SECRET))
        mode += "+s";
    if (hasMode(Channel::PROTECTED_TOPIC))
        mode += "+t";
    if (hasMode(Channel::NO_EXTERNAL_MESSAGES))
        mode += "+n";
    return mode;
}

int Channel::getUserCount()
{
    return users_.size();
}

bool Channel::isUserBanned(User *user)
{
    return ban_.find(user->nickmask) != ban_.end();
}

void Channel::addBan(User *user)
{
    ban_[user->nickmask] = user;
}

void Channel::removeBan(User *user)
{
    ban_.erase(user->nickmask);
}

bool Channel::isUserOnExceptionList(User *user)
{
    return exceptionList_.find(user->nickmask) != exceptionList_.end();
}

void Channel::addException(User *user)
{
    exceptionList_[user->nickmask] = user;
}

void Channel::removeException(User *user)
{
    exceptionList_.erase(user->nickmask);
}

bool Channel::isOnInviteList(User *user)
{
    return inviteList_.find(user->nickmask) != inviteList_.end();
}

void Channel::addInvite(User *user)
{
    inviteList_[user->nickmask] = user;
}

void Channel::removeInvite(User *user)
{
    inviteList_.erase(user->nickmask);
}

void Channel::broadcast(std::string const &message, User *sender)
{
    for (std::map<int, User *>::iterator it = users_.begin(); it != users_.end(); ++it)
    {
        if (it->second == sender)
            continue;
        it->second->send(message);
    }
}
