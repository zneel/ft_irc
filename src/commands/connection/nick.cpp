#include "../CommandManager.h"

bool isErrChar(std::string &nickname)
{
    for (std::string::iterator it = nickname.begin(); it != nickname.end(); it++)
    {
        if (*it == ' ' || *it == ':' || (it == nickname.begin() && *it == '#'))
            return true;
    }
    return false;
}

std::string nick(Message &msg, User *user, UserManager *uManager)
{
    if (msg.parameters.empty())
        return ERR_NONICKNAMEGIVEN("");
    else if (msg.parameters.length() > NICKLEN)
        msg.parameters.erase(NICKLEN, std::string::npos);
    else if (uManager->nickAlreadyUsed(msg.parameters))
        return ERR_NICKNAMEINUSE("", msg.parameters);
    else if (isErrChar(msg.parameters))
        return ERR_ERRONEUSNICKNAME("", "");
    else
        user->nick = msg.parameters;
    return "";
}
