#include "CommandManager.h"
#include "../user/UserManager.h"

CommandManager::CommandManager(ChannelManager *cManager, UserManager *uManager, std::string const &pwd)
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

void CommandManager::sendIsupport(User *sender)
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
        append(sender->getSendBuffer(), RPL_ISUPPORT(sender->nick, buf));
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
        append(sender->getSendBuffer(), RPL_ISUPPORT(sender->nick, buf));
    }
}

void CommandManager::doCommands(std::deque<Message> &msgs, User *sender)
{
    (void)cManager_;
    while (!msgs.empty())
    {
        std::cout << "Command: " << msgs.front().command << std::endl;
        std::cout << "Params: " << msgs.front().parameters << std::endl;
        if (msgs.front().command.compare("CAP") == 0)
            append(sender->getSendBuffer(), cap(msgs.front(), sender));
        else if (msgs.front().command.compare("PASS") == 0)
            append(sender->getSendBuffer(), pass(msgs.front(), sender, pwd_));
        else if (sender->isPassSent() && msgs.front().command.compare("NICK") == 0)
            append(sender->getSendBuffer(), nick(msgs.front(), sender, uManager_));
        else if (sender->isPassSent() && msgs.front().command.compare("USER") == 0)
            append(sender->getSendBuffer(), user(msgs.front(), sender));
        else if (sender->isRegistered())
        {
            if (msgs.front().command.compare("PING") == 0)
                append(sender->getSendBuffer(), ping(msgs.front(), sender));
        }
        msgs.pop_front();
        if (!sender->isRegistered() && sender->isPassSent() && !sender->nick.empty() && !sender->username.empty() &&
            !sender->realname.empty())
        {
            sender->setRegistered(true);
            append(sender->getSendBuffer(), RPL_WELCOME(sender->username, sender->nick, sender->ip + "@localhost"));
            sendIsupport(sender);
        }
    }
}

void CommandManager::append(std::string &sendBuffer, std::string toAdd)
{
    if (toAdd.empty())
        return;
    if (toAdd.length() > MAX_SIZE_SEND_BUFFER)
        toAdd.erase(MAX_SIZE_SEND_BUFFER, std::string::npos);
    sendBuffer.append(toAdd + CRLF);
}
