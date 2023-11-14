#include "CommandManager.h"
#include <sys/socket.h>

std::string privmsg(Message &msg, Client *sender, ClientManager *uManager, ChannelManager *cManager)
{
    if (msg.params.empty())
        return ERR_NEEDMOREPARAMS(sender->nick, msg.verb);
    std::string ret;
    while (!msg.params.empty())
    {
        std::string destName = msg.params.front();
        std::string userMessage = msg.trailling;
        if (*(msg.params.front().begin()) == '#')
        {
            if (cManager->get(destName) == NULL)
            {
                if (!ret.empty())
                    ret.append(CRLF);
                ret.append(ERR_NOSUCHCHANNEL(sender->nick, msg.params.front()));
            }
            else
            {
                Channel *channel = cManager->get(destName);
                if (channel->hasMode(Channel::BAN))
                {
                    std::string message = ":" + sender->RolePrefixToString(sender->getRoleInChannel(channel->name)) +
                                          sender->nickmask + " PRIVMSG " + channel->name + " :" + userMessage;
                    // if (channel->hasMode(Channel::EXCEPTION) && channel->isUserOnExceptionList(sender))
                    //     channel->broadcast(message);
                    // else if (channel->isUserBanned(sender))
                    //     return ERR_CANNOTSENDTOCHAN(sender->nick, msg.params);
                    // else
                    channel->broadcast(message, sender);
                }
            }
        }
        else
        {
            bool isFind = false;
            std::map<int, Client *> clients = uManager->getClients();
            for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
            {
                if (it->second->nick.compare(destName) == 0)
                {
                    isFind = true;
                    if (sender->isInPrivmsg(it->second->nick) == false)
                        sender->addPrivmsg(it->second);
                    if (it->second->isInPrivmsg(sender->nick) == false)
                        it->second->addPrivmsg(sender);
                    std::string message =
                        ":" + sender->nickmask + " PRIVMSG " + it->second->nickmask + " :" + userMessage;
                    it->second->send(message);
                }
            }
            if (isFind == false)
            {
                if (!ret.empty())
                    ret.append(CRLF);
                ret.append(ERR_NOSUCHNICK(sender->nick, destName));
            }
        }
        msg.params.pop_front();
    }
    return ret;
}
