#include "../CommandManager.h"
#include <sstream>

std::string oper(Message &msg, User *user, UserManager uManager, std::string opPassword)
{
    if (msg.parameters.find(' ') == std::string::npos)
        return ERR_NEEDMOREPARAMS(user->nick, msg.command);
    std::stringstream ss(msg.parameters);
    std::string nick, pwd;
    std::getline(ss, nick, ' ');
    std::getline(ss, pwd, ' ');
    if (uManager.nickAlreadyUsed(nick) == false)
        return ERR_NOOPERHOST(nick);
    if (pwd.compare(opPassword) != 0)
        return ERR_PASSWDMISMATCH(user->nick);
    user->setOp(true);
    return RPL_YOUREOPER(user->nick);
    // return MODE message too
}