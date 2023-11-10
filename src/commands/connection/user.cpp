#include "../CommandManager.h"

std::string user(Message &msg, User *user)
{
    if (!user->username.empty() && !user->realname.empty())
        return ERR_ALREADYREGISTERED("");
    if (!msg.params.empty())
    {
        user->username = std::string(msg.params[0]);
        user->realname = std::string(msg.trailling);
        return "";
    }
    return ERR_NEEDMOREPARAMS("", msg.verb);
}
