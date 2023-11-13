#include "CommandManager.h"
#include "../client/ClientManager.h"

CommandManager::CommandManager(ChannelManager *cManager, ClientManager *uManager, std::string const &pwd)
    : cManager_(cManager), uManager_(uManager), pwd_(pwd)
{
}

CommandManager::~CommandManager()
{
}

std::string toString(int nb)
{
    std::stringstream ss;
    ss << nb;
    std::string str = ss.str();
    return str;
}

void CommandManager::sendIsupport(Client *sender)
{
    {
        std::string buf;
        buf.append(" AWAYLEN=" + toString(AWAYLEN));
        buf.append(" CASEMAPPING=");
        buf.append(CASEMAPPING);
        buf.append(" CHANLIMIT=");
        buf.append(CHANLIMIT);
        buf.append(" CHANMODES=");
        buf.append(CHANMODES);
        buf.append(" CHANNELLEN=" + toString(CHANNELLEN));
        buf.append(" CHANTYPES=");
        buf.append(CHANTYPES);
        buf.append(" ELIST=");
        buf.append(ELIST);
        buf.append(" EXCEPTS=");
        buf.append(EXCEPTS);
        buf.append(" EXTBAN=");
        buf.append(EXTBAN);
        buf.append(" HOSTLEN=" + toString(HOSTLEN));
        buf.append(" INVEX=");
        buf.append(INVEX);
        buf.append(" KICKLEN=" + toString(KICKLEN));
        sender->send(SERVER_NAME + RPL_ISUPPORT(sender->nick, buf));
    }
    {
        std::string buf;
        buf.append(" MAXLIST=");
        buf.append(MAXLIST);
        buf.append(" MAXTARGET=" + toString(MAXTARGET));
        buf.append(" MODES=");
        buf.append(MODES);
        buf.append(" NETWORK=");
        buf.append(NETWORK);
        buf.append(" NICKLEN=" + toString(NICKLEN));
        buf.append(" PREFIX=");
        buf.append(PREFIX);
        buf.append(" SAFELIST");
        buf.append(SAFELIST);
        buf.append(" SILENCE=" + toString(SILENCE));
        buf.append(" STATUSMSG=");
        buf.append(STATUSMSG);
        buf.append(" TARGMAX=");
        buf.append(TARGMAX);
        buf.append(" TOPICLEN=" + toString(TOPICLEN));
        buf.append(" USERLEN=" + toString(USERLEN));
        sender->send(SERVER_NAME + RPL_ISUPPORT(sender->nick, buf));
    }
}

void CommandManager::sendMotd(Client *sender)
{
    sender->send(motd(LINE1, sender));
    sender->send(motd(LINE2, sender));
    sender->send(motd(LINE3, sender));
    sender->send(motd(LINE4, sender));
    sender->send(motd(LINE5, sender));
    sender->send(motd(LINE6, sender));
    sender->send(motd(LINE7, sender));
    sender->send(motd(LINE8, sender));
    sender->send(motd(LINE9, sender));
    sender->send(motd(LINE10, sender));
    sender->send(motd(LINE11, sender));
}

void CommandManager::doCommands(std::deque<Message> &msgs, Client *sender)
{
    while (!msgs.empty())
    {
        if (msgs.front().verb.compare("CAP") == 0)
            sender->send(cap(msgs.front(), sender));
        else if (msgs.front().verb.compare("PASS") == 0)
            sender->send(pass(msgs.front(), sender, pwd_));
        else if (sender->isPassSent() && msgs.front().verb.compare("NICK") == 0)
            sender->send(nick(msgs.front(), sender, uManager_, cManager_));
        else if (sender->isPassSent() && msgs.front().verb.compare("USER") == 0)
            sender->send(user(msgs.front(), sender));
        else if (sender->isRegistered())
        {
            if (msgs.front().verb.compare("motd") == 0)
                sendMotd(sender);
            else if (msgs.front().verb.compare("PING") == 0)
                sender->send(ping(msgs.front(), sender));
            else if (msgs.front().verb.compare("JOIN") == 0)
                sender->sendMany(join(msgs.front(), sender, cManager_));
            else if (msgs.front().verb.compare("PART") == 0)
                sender->sendMany(part(msgs.front(), sender, cManager_));
            else if (msgs.front().verb.compare("PRIVMSG") == 0)
                sender->send(privmsg(msgs.front(), sender, cManager_));
            else if (msgs.front().verb.compare("QUIT") == 0)
                sender->send(quit(msgs.front(), sender, cManager_));
        }
        msgs.pop_front();
        if (!sender->isRegistered() && sender->isPassSent() && !sender->nick.empty() && !sender->username.empty())
        {
            sender->setRegistered(true);
            sendIsupport(sender);
            sender->send(SERVER_NAME + RPL_WELCOME(sender->nick, sender->nickmask));
            sendMotd(sender);
        }
    }
}
