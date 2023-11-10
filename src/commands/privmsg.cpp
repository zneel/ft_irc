#include "CommandManager.h"

std::string privmsg(Message &msg, Client *sender, ChannelManager *cManager)
{
    if (msg.params.empty())
        return ERR_NEEDMOREPARAMS(sender->nick, msg.verb);
    if (msg.params[0][0] == '#')
    {

        std::string channelName = msg.params[0];
        std::string userMessage = msg.trailling;
        if (cManager->get(channelName) == NULL)
            return ERR_NOSUCHCHANNEL(sender->nick, msg.params[0]);
        Channel *channel = cManager->get(channelName);
        if (channel->hasMode(Channel::BAN))
        {
            std::string message = ":" + sender->RolePrefixToString(sender->getRoleInChannel(channel->name)) +
                                  sender->nickmask + " PRIVMSG " + channel->name + " :" + userMessage;
            // if (channel->hasMode(Channel::EXCEPTION) && channel->isUserOnExceptionList(sender))
            //     channel->broadcast(message);
            // else if (channel->isUserBanned(sender))
            //     return ERR_CANNOTSENDTOCHAN(sender->nick, msg.parameters);
            // else
            channel->broadcast(message, sender);
        }
    }
    return "";
}
