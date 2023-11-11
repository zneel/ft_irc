#include "Channel.h"

Channel::Channel(std::string name, ChannelMode mode, Type type)
    : name(name), password(""), topic(""), modes_(mode), type_(type)
{
}

Channel::~Channel()
{
}

std::map<int, Client *> Channel::getClients()
{
    return clients_;
}

std::map<int, Client *> Channel::getOperators()
{
    return operators_;
}

int Channel::getMode()
{
    return modes_;
}

void Channel::setMode(int mode)
{
    modes_ = mode;
}

void Channel::addMode(int mode)
{
    modes_ |= mode;
}

bool Channel::hasMode(int mode)
{
    return modes_ & mode;
}

bool Channel::hasModes(int modes)
{
    return (modes_ & modes) == modes;
}

Channel::ChannelMode Channel::getMode(char mode) const
{
    switch (mode)
    {
    case 'b':
        return (BAN);
    case 'e':
        return (EXCEPTION);
    case 'l':
        return (CLIENT_LIMIT);
    case 'i':
        return (INVITE_ONLY);
    case 'I':
        return (INVITE_EXCEPTION);
    case 'k':
        return (KEY);
    case 'm':
        return (MODERATED);
    case 's':
        return (SECRET);
    case 't':
        return (PROTECTED_TOPIC);
    case 'n':
        return (NO_EXTERNAL_MESSAGES);
    default:
        return (NOT_SUPPORTED);
    }
}

void Channel::removeMode(int mode)
{
    modes_ &= ~mode;
}

std::string Channel::modesToStr()
{
    std::string ret;
    ret += "+";
    if (hasMode(Channel::BAN))
        ret += "b";
    if (hasMode(Channel::EXCEPTION))
        ret += "e";
    if (hasMode(Channel::CLIENT_LIMIT))
        ret += "l";
    if (hasMode(Channel::INVITE_ONLY))
        ret += "i";
    if (hasMode(Channel::INVITE_EXCEPTION))
        ret += "I";
    if (hasMode(Channel::KEY))
        ret += "k";
    if (hasMode(Channel::MODERATED))
        ret += "m";
    if (hasMode(Channel::SECRET))
        ret += "s";
    if (hasMode(Channel::PROTECTED_TOPIC))
        ret += "t";
    if (hasMode(Channel::NO_EXTERNAL_MESSAGES))
        ret += "n";
    return ret;
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

void Channel::addClient(Client *client)
{
    clients_[client->getFd()] = client;
}

void Channel::removeClient(Client *client)
{
    clients_.erase(client->getFd());
}

void Channel::addOperator(Client *client)
{
    operators_[client->getFd()] = client;
}

void Channel::removeOperator(Client *client)
{
    operators_.erase(client->getFd());
}

bool Channel::isOperator(Client *client)
{
    return operators_.find(client->getFd()) != operators_.end();
}

std::string Channel::modeToStr()
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

int Channel::getClientCount()
{
    return clients_.size();
}

bool Channel::isClientBanned(Client *client)
{
    return ban_.find(client->nickmask) != ban_.end();
}

void Channel::addBan(Client *client)
{
    ban_[client->nickmask] = client;
}

void Channel::removeBan(Client *client)
{
    ban_.erase(client->nickmask);
}

bool Channel::isClientOnExceptionList(Client *client)
{
    return exceptionList_.find(client->nickmask) != exceptionList_.end();
}

void Channel::addException(Client *client)
{
    exceptionList_[client->nickmask] = client;
}

void Channel::removeException(Client *client)
{
    exceptionList_.erase(client->nickmask);
}

bool Channel::isOnInviteList(Client *client)
{
    return inviteList_.find(client->nickmask) != inviteList_.end();
}

void Channel::addInvite(Client *client)
{
    inviteList_[client->nickmask] = client;
}

void Channel::removeInvite(Client *client)
{
    inviteList_.erase(client->nickmask);
}

void Channel::broadcast(std::string const &message, Client *sender, bool sendToSender)
{
    for (std::map<int, Client *>::iterator it = clients_.begin(); it != clients_.end(); ++it)
    {
        if (it->second != sender || sendToSender)
            it->second->send(message);
    }
}

bool Channel::isClientOnChannel(Client *client)
{
    return clients_.find(client->getFd()) != clients_.end();
}
