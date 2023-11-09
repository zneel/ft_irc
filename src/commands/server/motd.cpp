#include "../CommandManager.h"

std::string motd(std::string const &msg, User *user)
{
    std::string motd_;
    motd_.append(SERVER_NAME + RPL_MOTD(user->nick, msg));
    return motd_;
}
