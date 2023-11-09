#include "CommandManager.h"

std::string privmsg(Message &msg, Client *sender, ChannelManager *cManager)
{
    if (msg.parameters.size() < 2)
        return ERR_NEEDMOREPARAMS(sender->nick, msg.command);
    if (msg.parameters[0] == '#')
    {

        std::string channelName = msg.parameters.substr(0, msg.parameters.find_first_of(" ", 0));
        std::string userMessage = msg.parameters.substr(msg.parameters.find_first_of(" ", 0) + 1);
        if (cManager->get(channelName) == NULL)
            return ERR_NOSUCHCHANNEL(sender->nick, msg.parameters);
        Channel *channel = cManager->get(channelName);
        if (channel->hasMode(Channel::BAN))
        {
            std::string message = ":" + sender->nickmask + " PRIVMSG " + channel->name + " :" + userMessage;
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
