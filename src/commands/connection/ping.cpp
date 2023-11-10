#include "../../ft_irc.h"
#include "../CommandManager.h"

std::string ping(Message &msg, User *user)
{
    if (msg.trailling.empty())
        return ERR_NEEDMOREPARAMS(user->nick, "PING");
    return "PONG " + msg.trailling + " :" + user->nick;
}
