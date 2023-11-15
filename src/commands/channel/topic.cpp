#include "../CommandManager.h"

std::string topic(Message &msg, Client *user, ChannelManager *cManager)
{
    if (msg.params.empty())
        return SERVER_NAME + ERR_NEEDMOREPARAMS(user->nick, msg.verb);
    Channel *chan = cManager->get(*(msg.params.begin()));
    if (msg.trailling.empty())
    {
        if (msg.isEmptyTrailling)
        {
            if (chan->hasMode(8))
            {
                if (user->getModes() == 8)
                    chan->topic = "";
                else
                    return SERVER_NAME + ERR_CHANOPRIVSNEEDED(user->nick, chan->name);
            }
            else
                chan->topic = "";
        }
        else
        {
            if (chan->topic.empty())
                return RPL_NOTOPIC(user->nick, chan->name);
            else
                return RPL_TOPIC(user->nick, chan->name, chan->topic);
        }
    }
    else
    {
        if (chan->hasMode(8))
        {
            if (user->getModes() == 8)
                chan->topic = msg.trailling;
            else
                return SERVER_NAME + ERR_CHANOPRIVSNEEDED(user->nick, chan->name);
        }
        else
            chan->topic = msg.trailling;
    }
	chan->broadcast(":" + user->nickmask + " TOPIC " + chan->name + " " + chan->topic, user, true);
    return "";
}