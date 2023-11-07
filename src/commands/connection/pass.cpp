#include "../CommandManager.h"

std::string pass(Message &msg, User *user, std::string &serverPassword)
{
    if (msg.parameters.empty())
        return ERR_NEEDMOREPARAMS(user->getNick(), "PASS");
    if (user->isRegistered())
        return ERR_ALREADYREGISTERED(user->getNick());
    if (msg.parameters.compare(serverPassword) != 0)
    {
        user->setRegistered(false);
        return ERR_PASSWDMISMATCH(user.getNick());
    }
    user->setPassSent(true);
    return "";
}
