#include "ChannelManager.h"

ChannelManager::ChannelManager()
{
}

ChannelManager::~ChannelManager()
{
}

Channel *ChannelManager::get(std::string &name) const
{
    return channels_.find(name)->second;
}

Channel *ChannelManager::create(std::string name, int mode, int type)
{
    Channel *channel = new Channel(name, mode, type);
    channels_.insert(std::pair<std::string, Channel *>(name, channel));
    return channel;
}

void ChannelManager::remove(std::string name)
{
    delete channels_.find(name)->second;
    channels_.erase(name);
}

void ChannelManager::removeAll()
{
    for (std::map<std::string, Channel *>::iterator it = channels_.begin(); it != channels_.end(); ++it)
        delete it->second;
    channels_.clear();
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
