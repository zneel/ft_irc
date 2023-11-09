#include "../CommandManager.h"

std::string user(Message &msg, Client *user)
{
    if (!user->username.empty() && !user->realname.empty())
        return SERVER_NAME + ERR_ALREADYREGISTERED("");
    if (msg.parameters.find(" ") != std::string::npos && msg.parameters.find(":") != std::string::npos)
    {
        std::string::size_type pos = msg.parameters.find(" ");
        std::string::size_type pos2 = msg.parameters.find(":") + 1;
        user->username = std::string(msg.parameters, 0, pos);
        user->realname = std::string(msg.parameters, pos2, std::string::npos);
        return "";
    }
    return SERVER_NAME + ERR_NEEDMOREPARAMS("", msg.command);
}
