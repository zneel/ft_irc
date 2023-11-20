#include "../client/Bot.h"
#include "CommandManager.h"

bool doBotThings(Message msg, Client *user, ClientManager *uManager, ChannelManager *cManager)
{
    (void)uManager;
    bool isAction = false;
    while (!msg.params.empty())
    {
        if (msg.verb.compare("PRIVMSG") == 0 && *(msg.params.front().begin()) == '#' &&
            msg.trailling.compare("!help") == 0)
        {
            std::string channelName = msg.params.front();
			if (cManager->get(channelName) == NULL)
				return false;
            Bot bot;
            std::string prefix(":" + bot.nickmask + " PRIVMSG " + user->nickmask + " :");
            std::string botMessage;
            botMessage.append(prefix + "Of course I can help you " + user->nick + " !" + CRLF);
            botMessage.append(prefix + "CHANNEL COMMAND on irssi for channel: " + channelName + CRLF);
            if (cManager->get(channelName)->isOperator(user))
            {
                    botMessage.append(std::string(prefix + "") + CRLF);
                    botMessage.append(std::string(prefix + "As an operator you can do:") + CRLF);
                    botMessage.append(std::string(prefix + "/mode <mod> => to change the mode of the channel (l i k t o)") + CRLF);
                    botMessage.append(std::string(prefix + "/invite <nickToInvite> => to invite someone if the channel is on invite mode") + CRLF);
                    botMessage.append(std::string(prefix + "/topic <newTopic> => to set the channel topic") + CRLF);
                    botMessage.append(std::string(prefix + "/kick <user> => to kick an user from the channel") + CRLF);
            }
            botMessage.append(std::string(prefix + "") + CRLF);
            botMessage.append(std::string(prefix + "As a normal user you can do:") + CRLF);
            botMessage.append(std::string(prefix + "/topic => to see the channel topic") + CRLF);
            botMessage.append(std::string(prefix + "/names => to see users in the channel") + CRLF);
            botMessage.append(std::string(prefix + "/part => to leave this channel") + CRLF);
            Message msgCopy = msg;
            user->getSendBuffer().append(botMessage);
            isAction = true;
        }
        msg.params.pop_front();
    }
    return isAction;
}

