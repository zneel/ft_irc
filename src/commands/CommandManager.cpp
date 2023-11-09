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
    (void)cManager_;
    while (!msgs.empty())
    {
        std::cout << "Command: " << msgs.front().command << std::endl;
        std::cout << "Params : " << msgs.front().parameters << std::endl;
        if (msgs.front().command.compare("CAP") == 0)
            sender->send(cap(msgs.front(), sender));
        else if (msgs.front().command.compare("PASS") == 0)
            sender->send(pass(msgs.front(), sender, pwd_));
        else if (sender->isPassSent() && msgs.front().command.compare("NICK") == 0)
            sender->send(nick(msgs.front(), sender, uManager_));
        else if (sender->isPassSent() && msgs.front().command.compare("USER") == 0)
            sender->send(user(msgs.front(), sender));
        else if (sender->isRegistered())
        {
            if (msgs.front().command.compare("motd") == 0)
                sendMotd(sender);
            if (msgs.front().command.compare("PING") == 0)
                sender->send(ping(msgs.front(), sender));
            if (msgs.front().command.compare("JOIN") == 0)
                sender->send(join(msgs.front(), sender, cManager_));
            if (msgs.front().command.compare("PRIVMSG") == 0)
                sender->send(privmsg(msgs.front(), sender, cManager_));
        }
        msgs.pop_front();
        if (!sender->isRegistered() && sender->isPassSent() && !sender->nick.empty() && !sender->username.empty())
        {
            sender->setRegistered(true);
            sender->nickmask = sender->nick + "!" + sender->username + "@localhost";
            sendIsupport(sender);
            sender->send(SERVER_NAME + RPL_WELCOME(sender->username, sender->nickmask));
            sendMotd(sender);
        }
    }
}
