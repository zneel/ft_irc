#include "../../ft_irc.h"
#include "../CommandManager.h"

std::string ping(Message &msg, User *user)
{
    std::string token;

    if (msg.parameters.empty())
        return ERR_NEEDMOREPARAMS(user->nick, "PING");
    token = msg.parameters.substr(0, msg.parameters.find(" "));
    return "PONG " + token + " :" + user->nick;
}
