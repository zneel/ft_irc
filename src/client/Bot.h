#pragma once

#include "../commands/CommandManager.h"

class Bot : public Client
{
  public:
    Bot();
    ~Bot();

    bool doBotThings(Message msg, Client *user, ClientManager *uManager, ChannelManager *cManager);
};