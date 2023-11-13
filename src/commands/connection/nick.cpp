#include "../CommandManager.h"

bool isErrChar(std::string &nickname)
{
    if (nickname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-") !=
        std::string::npos)
        return true;
    return false;
}

std::string nick(Message &msg, Client *user, ClientManager *uManager, ChannelManager *cManager)
{
    std::string ret = "";
    if (msg.params.empty())
        return SERVER_NAME + ERR_NONICKNAMEGIVEN("");
    else if (msg.params[0].length() > NICKLEN)
        msg.params[0].erase(NICKLEN, std::string::npos);
    if (uManager->nickAlreadyUsed(msg.params[0]))
        return SERVER_NAME + ERR_NICKNAMEINUSE(msg.params[0], msg.params[0]);
    if (isErrChar(msg.params[0]))
        return SERVER_NAME + ERR_ERRONEUSNICKNAME(user->nick, msg.params[0]);
    std::map<std::string, Channel *> channels = cManager->getAll();
    ret = ":" + user->nickmask + " NICK " + msg.params[0];
    for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if (it->second->isClientOnChannel(user))
        {
            it->second->broadcast(ret, user);
        }
    }
    user->nick = msg.params[0];
    user->updtadeNickmask();
    return ret;
}
