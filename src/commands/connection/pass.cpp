#include "../CommandManager.h"

std::string pass(Message &msg, User *user, std::string &serverPassword)
{
    if (user->isRegistered())
        return ERR_ALREADYREGISTERED(user->nick);
    if (msg.parameters.empty())
        return ERR_NEEDMOREPARAMS(user->nick, "PASS");
    if (msg.parameters.compare(serverPassword) != 0)
    {
        user->setShouldDisconnect(true);
        return ERR_PASSWDMISMATCH(user->nick);
    }
    user->setPassSent(true);
    user->setShouldDisconnect(false);
    return "";
}
