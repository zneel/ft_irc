#include "../CommandManager.h"

std::string user(Message &msg, User *user)
{
    if (!user->username.empty() && !user->realname.empty())
        return ERR_ALREADYREGISTERED("");
    if (msg.params.size() >= 3)
    {
        user->username = std::string(msg.params[0]);
        user->realname = std::string(msg.params[2]);
        return "";
    }
    std::cout << msg.params.size() << std::endl;
    return ERR_NEEDMOREPARAMS("", msg.verb);
}
