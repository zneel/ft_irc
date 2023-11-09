#include "../../ft_irc.h"
#include "../CommandManager.h"

std::string ping(Message &msg, User *user)
{
    if (msg.params.size() < 1)
        return ERR_NEEDMOREPARAMS(user->nick, "PING");
    return "PONG " + msg.params[0] + " :" + user->nick;
}
