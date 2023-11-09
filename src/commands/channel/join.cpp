#include "../../utils/utils.h"
#include "../CommandManager.h"

#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct ChannelParsing
{
    std::string channelName;
    std::string channelPassword;
    Channel::Type type;
};

static std::vector<ChannelParsing> parseJoin(std::string const &channels, std::string const &passwords)
{
    std::vector<ChannelParsing> parsed;
    std::vector<std::string> splittedChannels = split(channels, ",");
    std::vector<std::string> splittedPasswords = split(passwords, ",");
    for (size_t i = 0; i < splittedChannels.size(); i++)
    {
        ChannelParsing tmp;
        tmp.channelName = splittedChannels[i];
        if (i < splittedPasswords.size())
            tmp.channelPassword = splittedPasswords[i];
        else
            tmp.channelPassword = "";
        tmp.type = tmp.channelName[0] == '#' ? Channel::REGULAR : Channel::LOCAL;
        parsed.push_back(tmp);
    }
    return parsed;
}

std::string join(Message &msg, User *user, ChannelManager *cManager)
{
    if (msg.parameters.empty())
        return SERVER_NAME + ERR_NEEDMOREPARAMS(user->nick, msg.command);
    std::vector<std::string> splitted = split(msg.parameters, " ");
    if (splitted.size() > 2)
        return SERVER_NAME + ERR_NEEDMOREPARAMS(user->nick, msg.command);
    std::vector<ChannelParsing> parsed = parseJoin(splitted[0], splitted.size() == 2 ? splitted[1] : "");
    std::map<std::string, Channel *> channels = cManager->getAll();
    for (std::vector<ChannelParsing>::iterator it = parsed.begin(); it != parsed.end(); ++it)
    {
        std::string broadcastMessage = ":" + user->nickmask + " JOIN " + it->channelName;
        if (it->channelName[0] != '#' && it->channelName[0] != '&')
            return SERVER_NAME + ERR_NOSUCHCHANNEL(user->nick, it->channelName);

        if (it->channelName.length() <= 1 || it->channelName.length() > CHANNELLEN)
            return SERVER_NAME + ERR_NOSUCHCHANNEL(user->nick, it->channelName);

        if (it->channelName.find_first_of(", \a") != std::string::npos)
            return SERVER_NAME + ERR_NOSUCHCHANNEL(user->nick, it->channelName);

        if (it->channelPassword.find_first_of(", \a") != std::string::npos)
            return SERVER_NAME + ERR_INVALIDKEY(user->nick, it->channelName);

        if (channels.find(it->channelName) == channels.end())
        {
            Channel *newChannel = cManager->create(it->channelName, Channel::BAN, it->type);
            if (!it->channelPassword.empty())
                newChannel->password = it->channelPassword;
            newChannel->addUser(user);
            newChannel->addOperator(user);
            newChannel->broadcast(broadcastMessage, user, true);
        }
        else
        {
            Channel *channel = channels.find(it->channelName)->second;
            if (!channel)
                return SERVER_NAME + ERR_NOSUCHCHANNEL(user->nick, it->channelName);
            if (channel->hasMode(Channel::BAN))
            {
                if (channel->hasMode(Channel::EXCEPTION) && !channel->isUserOnExceptionList(user) &&
                    channel->isUserBanned(user))
                {
                    return SERVER_NAME + ERR_BANNEDFROMCHAN(user->nick, channel->name);
                }
                else if (!channel->isUserBanned(user))
                {
                    channel->addUser(user);
                    channel->broadcast(broadcastMessage, user, true);
                }
                else
                    return SERVER_NAME + ERR_BANNEDFROMCHAN(user->nick, channel->name);
            }
            else if (channel->hasMode(Channel::CLIENT_LIMIT) && channel->getUserCount() >= channel->maxUser)
                return SERVER_NAME + ERR_CHANNELISFULL(user->nick, channel->name);

            else if (channel->hasMode(Channel::INVITE_ONLY) && !channel->isOnInviteList(user))
                return SERVER_NAME + ERR_INVITEONLYCHAN(user->nick, channel->name);

            else if (channel->hasMode(Channel::KEY) && channel->password != it->channelPassword)
                return SERVER_NAME + ERR_BADCHANNELKEY(user->nick, channel->name);

            else
            {
                channel->addUser(user);
                channel->broadcast(broadcastMessage, user, true);
            }
        }
    }
    return "";
}
