#pragma once

#include "../channel/ChannelManager.h"
#include "../ft_irc.h"
#include "../message/Message.h"
#include "../user/User.h"
#include "../user/UserManager.h"
#include <deque>
#include <string>

class CommandManager
{
  public:
    CommandManager(ChannelManager *cManager, UserManager *uManager, std::string const &pwd);
    ~CommandManager();

    void doCommands(std::deque<Message> &msgs, User *sender);
    void sendIsupport(User *sender);
    void append(std::string &sendBuffer, std::string toAdd);

  private:
    ChannelManager *cManager_;
    UserManager *uManager_;
    std::string pwd_;
};

std::string cap(Message &msg, User *user);
std::string pass(Message &msg, User *user, std::string &password);
std::string nick(Message &msg, User *user, UserManager *uManager);
std::string user(Message &msg, User *user);

std::string ping(Message &msg, User *user);
