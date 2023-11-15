#include "Bot.h"

Bot::Bot() : Client()
{
    nick = "BOT";
    username = "BOT";
    realname = "ROBOT";
    updateNickmask();
}

Bot::~Bot()
{
}

bool Bot::doBotThings(Message msg, Client *user, ClientManager *uManager, ChannelManager *cManager)
{
	bool isAction = false;
    while (!msg.params.empty())
    {
        if (msg.verb.compare("PRIVMSG") == 0 && *(msg.params.front().begin()) == '#' &&
            msg.trailling.compare("!help") == 0)
        {
            std::string channelName = msg.params.front();
            msg.params.front() = user->nick;
            {
                msg.trailling = "Of course I can help you " + user->nick + " !";
                Message msgCopy = msg;
                privmsg(msgCopy, this, uManager, cManager);
            }
            {
                msg.trailling = "CHANNEL COMMAND on irssi:";
                Message msgCopy = msg;
                privmsg(msgCopy, this, uManager, cManager);
            }
            if (cManager->get(channelName)->isOperator(user))
            {
                {
                    msg.trailling = "";
                    Message msgCopy = msg;
                    privmsg(msgCopy, this, uManager, cManager);
                }
                {
                    msg.trailling = "As an operator you can do:";
                    Message msgCopy = msg;
                    privmsg(msgCopy, this, uManager, cManager);
                }
                {
                    msg.trailling = "/mode <mod> => to change the mode of the channel (l i k t o)";
                    Message msgCopy = msg;
                    privmsg(msgCopy, this, uManager, cManager);
                }
                {
                    msg.trailling = "/invite <nickToInvite> => to invite someone if the channel is on invite mode";
                    Message msgCopy = msg;
                    privmsg(msgCopy, this, uManager, cManager);
                }
                {
                    msg.trailling = "/topic <newTopic> => to set the channel topic";
                    Message msgCopy = msg;
                    privmsg(msgCopy, this, uManager, cManager);
                }
                {
                    msg.trailling = "/kick <user> => to kick an user from the channel";
                    Message msgCopy = msg;
                    privmsg(msgCopy, this, uManager, cManager);
                }
            }
            {
                msg.trailling = "";
                Message msgCopy = msg;
                privmsg(msgCopy, this, uManager, cManager);
            }
            {
                msg.trailling = "As a normal user you can do:";
                Message msgCopy = msg;
                privmsg(msgCopy, this, uManager, cManager);
            }
            {
                msg.trailling = "/topic => to see the channel topic";
                Message msgCopy = msg;
                privmsg(msgCopy, this, uManager, cManager);
            }
            {
                msg.trailling = "/names => to see users in the channel";
                Message msgCopy = msg;
                privmsg(msgCopy, this, uManager, cManager);
            }
            {
                msg.trailling = "/part => to leave this channel";
                Message msgCopy = msg;
                privmsg(msgCopy, this, uManager, cManager);
            }
			isAction = true;
        }
        msg.params.pop_front();
    }
    return isAction;
}
