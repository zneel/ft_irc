#include "../CommandManager.h"
#include <cctype>
#include <vector>

struct Mode
{
    char mode;
    bool set;
};

std::vector<Mode> parseModeStr(std::string const &modeStr)
{
    std::vector<Mode> modes;
    bool inMode = false;
    Mode mode;
    for (std::string::const_iterator it = modeStr.begin(); it != modeStr.end(); ++it)
    {
        if (!inMode)
            mode = Mode();
        if (!inMode && (*it == '+' || *it == '-'))
        {
            mode.set = *it == '+';
            inMode = true;
            continue;
        }
        mode.mode = *it;
        modes.push_back(mode);
        inMode = false;
    }
    return modes;
}

std::vector<std::string> mode(Message &msg, Client *client, ClientManager *cManager, ChannelManager *chManager)
{
    std::string const USER_MODES = "+-ioOrw";
    std::string const CHANNEL_MODES = "+-beliIkmstn";
    std::vector<std::string> ret;
    if (msg.params.empty() || msg.params[0].empty())
        ret.push_back(SERVER_NAME + ERR_NEEDMOREPARAMS(client->nick, "MODE"));
    else if (msg.params[0][0] != '#')
    {
        if (!cManager->nickExists(msg.params[0]))
            ret.push_back(SERVER_NAME + ERR_NOSUCHNICK(client->nick, msg.params[0]));
        else if (!cManager->isSameClient(client, msg.params[0]))
            ret.push_back(SERVER_NAME + ERR_USERSDONTMATCH(client->nick));
        else if (msg.params.size() == 1)
            ret.push_back(SERVER_NAME + RPL_UMODEIS(client->nick, client->modesToStr()));
        else
        {
            std::vector<Mode> modes = parseModeStr(msg.params[1]);
            for (std::vector<Mode>::const_iterator it = modes.begin(); it != modes.end(); ++it)
            {
                if (USER_MODES.find(it->mode) == std::string::npos)
                {
                    ret.push_back(SERVER_NAME + ERR_UMODEUNKNOWNFLAG(client->nick));
                    continue;
                }
                if (it->set)
                    client->addMode(client->getMode(it->mode));
                else
                    client->removeMode(client->getMode(it->mode));
            }
            ret.push_back(SERVER_NAME + RPL_UMODEIS(client->nick, client->modesToStr()));
        }
    }
    else if (msg.params[0][0] == '#')
    {
        if (!chManager->channelExists(msg.params[0]))
            ret.push_back(SERVER_NAME + ERR_NOSUCHCHANNEL(client->nick, msg.params[0]));

        else if (msg.params.size() == 1)
        {
            Channel *chan = chManager->get(msg.params[0]);
            ret.push_back(SERVER_NAME + RPL_CHANNELMODEIS(client->nick, msg.params[0], chan->modesToStr()));
        }
        else
        {
            Channel *chan = chManager->get(msg.params[0]);
            if (!chan->isClientOnChannel(client))
                ret.push_back(SERVER_NAME + ERR_NOTONCHANNEL(client->nick, msg.params[0]));
            else if (!chan->isOperator(client))
                ret.push_back(SERVER_NAME + ERR_CHANOPRIVSNEEDED(client->nick, msg.params[0]));
            else
            {
                std::vector<Mode> modes = parseModeStr(msg.params[1]);
                for (std::vector<Mode>::const_iterator it = modes.begin(); it != modes.end(); ++it)
                {
                    if (CHANNEL_MODES.find(it->mode) == std::string::npos)
                    {
                        ret.push_back(SERVER_NAME + ERR_UNKNOWNMODE(client->nick, std::string(1, it->mode)));
                        continue;
                    }
                    if (it->set)
                        chan->addMode(chan->getMode(it->mode));
                    else
                        chan->removeMode(chan->getMode(it->mode));
                }
                chan->broadcast(SERVER_NAME + RPL_CHANNELMODEIS(client->nick, msg.params[0], chan->modesToStr()),
                                client, true);
            }
        }
    }

    return ret;
}
