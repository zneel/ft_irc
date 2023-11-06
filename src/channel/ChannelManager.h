#pragma once

#include "Channel.h"
#include <string>

class ChannelManager
{
  public:
    ChannelManager();
    ~ChannelManager();

    Channel *get(std::string &name) const;
    Channel *create(std::string name, int mode, int type);
    void remove(std::string name);
    void removeAll();

  private:
    ChannelManager &operator=(ChannelManager const &rhs);
    ChannelManager(ChannelManager const &other);
    std::map<std::string, Channel *> channels_;
};
