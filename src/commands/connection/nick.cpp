#include "../CommandManager.h"

bool isErrChar(std::string &nickname)
{
    if (nickname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-") !=
        std::string::npos)
        return true;
    return false;
}

std::string nick(Message &msg, Client *user, ClientManager *uManager)
{
    std::string oldNick(user->nick);
    if (msg.params.empty())
        return SERVER_NAME + ERR_NONICKNAMEGIVEN("");
    else if (msg.params[0].length() > NICKLEN)
        msg.params[0].erase(NICKLEN, std::string::npos);
    if (uManager->nickAlreadyUsed(msg.params[0]))
        return SERVER_NAME + ERR_NICKNAMEINUSE(msg.params[0], msg.params[0]);
    if (isErrChar(msg.params[0]))
        return SERVER_NAME + ERR_ERRONEUSNICKNAME(user->nick, msg.params[0]);
    user->nick = msg.params[0];
     if (!oldNick.empty())
        return ":" + oldNick + " NICK " + user->nick + " ; " + oldNick + " changed his nickname to " + user->nick;
    return "";
}
