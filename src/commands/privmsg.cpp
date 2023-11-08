#include "CommandManager.h"

std::string privmsg(Message &msg, User *sender, ChannelManager *cManager)
{
    if (msg.parameters.size() < 2)
        return ERR_NEEDMOREPARAMS(sender->nick, msg.command);
    if (msg.parameters[0] == '#')
    {
        if (cManager->get(msg.parameters) == NULL)
            return ERR_NOSUCHCHANNEL(sender->nick, msg.parameters);
        Channel *channel = cManager->get(msg.parameters);
        if (channel->hasMode(Channel::BAN))
        {
            if (channel->hasMode(Channel::EXCEPTION) && channel->isUserOnExceptionList(sender))
            {
                std::string message =
                    ":" + sender->nickmask + " " + msg.command + " " + msg.parameters + " :" + msg.parameters[1];
                channel->broadcast(message, sender);
            }
            else
                return ERR_CANNOTSENDTOCHAN(sender->nick, msg.parameters);
        }
    }
    return "";
}
