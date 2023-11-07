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
        user->getSendBuffer().append(ERR_NONICKNAMEGIVEN(""));
    else if (uManager->nickAlreadyUsed(msg.parameters))
        user->getSendBuffer().append(ERR_NICKNAMEINUSE("", msg.parameters));
    else if (isErrChar(msg.parameters))
        user->getSendBuffer().append(ERR_ERRONEUSNICKNAME("", ""));
    else
        user->nick = msg.parameters;
    return "";
}
