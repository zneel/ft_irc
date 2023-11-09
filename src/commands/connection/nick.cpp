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
    if (msg.parameters.empty())
        return SERVER_NAME + ERR_NONICKNAMEGIVEN("");
    else if (msg.parameters.length() > NICKLEN)
        msg.parameters.erase(NICKLEN, std::string::npos);
    else if (uManager->nickAlreadyUsed(msg.parameters))
        return SERVER_NAME + ERR_NICKNAMEINUSE(msg.parameters, msg.parameters);
    else if (isErrChar(msg.parameters))
        return SERVER_NAME + ERR_ERRONEUSNICKNAME(user->nick, msg.parameters);
    else
        user->nick = msg.parameters;
    return "";
}
