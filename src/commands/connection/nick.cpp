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
    std::string oldNick(user->nick);
    if (msg.params.empty())
        return ERR_NONICKNAMEGIVEN("");
    else if (msg.params[0].length() > NICKLEN)
        msg.params[0].erase(NICKLEN, std::string::npos);
    if (uManager->nickAlreadyUsed(msg.params[0]))
        return ERR_NICKNAMEINUSE("", msg.params[0]);
    if (isErrChar(msg.params[0]))
        return ERR_ERRONEUSNICKNAME("", "");
    user->nick = msg.params[0];
    if (!oldNick.empty())
        return ":" + oldNick + " NICK " + user->nick;
    return "";
}
