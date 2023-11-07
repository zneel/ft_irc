#pragma once

#include "../channel/ChannelManager.h"
#include "../ft_irc.h"
#include "../message/Message.h"
#include "../user/User.h"
#include "../user/UserManager.h"
#include <deque>

class CommandManager
{
  public:
    CommandManager(ChannelManager *cManager, UserManager *uManager);
    ~CommandManager();

    void doCommands(std::deque<Message> &msgs, User *sender);
    void append(std::string &sendBuffer, std::string toAdd);

  private:
    ChannelManager *cManager_;
    UserManager *uManager_;
};

std::string cap(Message msg, User *user);