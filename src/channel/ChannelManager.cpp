#include "ChannelManager.h"
#include "Channel.h"

ChannelManager::ChannelManager()
{
    welcomeMessage_.append(LINE1) + CRLF;
    welcomeMessage_.append(LINE2) + CRLF;
    welcomeMessage_.append(LINE3) + CRLF;
    welcomeMessage_.append(LINE4) + CRLF;
    welcomeMessage_.append(LINE5) + CRLF;
    welcomeMessage_.append(LINE6) + CRLF;
    welcomeMessage_.append(LINE7) + CRLF;
    welcomeMessage_.append(LINE8) + CRLF;
    welcomeMessage_.append(LINE9) + CRLF;
    welcomeMessage_.append(LINE10) + CRLF;
    welcomeMessage_.append(LINE11) + CRLF;
}

ChannelManager::~ChannelManager()
{
    removeAll();
}

Channel *ChannelManager::get(std::string &name) const
{
    if (channels_.find(name) == channels_.end())
        return NULL;
    return channels_.find(name)->second;
}

Channel *ChannelManager::create(std::string name, Channel::Mode mode, Channel::Type type)
{
    Channel *channel = new Channel(name, mode, type);
    channels_[name] = channel;
    return channels_[name];
}

std::map<std::string, Channel *> ChannelManager::getAll() const
{
    return channels_;
}

void ChannelManager::remove(std::string name)
{
    ChannelMapIterator it = channels_.find(name);
    if (it != channels_.end())
    {
        delete it->second;
        channels_.erase(it);
    }
}

void ChannelManager::removeAll()
{
    for (ChannelMapIterator it = channels_.begin(); it != channels_.end(); ++it)
        delete it->second;
    channels_.clear();
}

void ChannelManager::broadcast(std::string const &message, Channel *channel)
{
    std::map<int, User *> users = channel->getUsers();
    for (std::map<int, User *>::iterator it = users.begin(); it != users.end(); ++it)
        it->second->send(message);
}

ChannelManager &ChannelManager::operator=(ChannelManager const &rhs)
{
    if (this != &rhs)
    {
        channels_ = rhs.channels_;
    }
    return *this;
}

ChannelManager::ChannelManager(ChannelManager const &other) : channels_(other.channels_)
{
}
