#include "CommandManager.h"
#include "../user/UserManager.h"

CommandManager::CommandManager(ChannelManager *cManager, UserManager *uManager, std::string const &pwd)
    : cManager_(cManager), uManager_(uManager), pwd_(pwd)
{
}

CommandManager::~CommandManager()
{
}

void CommandManager::sendIsupport(User *sender)
{
    if (sender->isupportTokenPackCount == 0)
    {
        std::string buf;
        buf.append(" AWAYLEN=" + std::to_string(AWAYLEN));
        buf.append(" CASEMAPPING=");
        buf.append(CASEMAPPING);
        buf.append(" CHANLIMIT=");
        buf.append(CHANLIMIT);
        buf.append(" CHANMODES=");
        buf.append(CHANMODES);
        buf.append(" CHANNELLEN=" + std::to_string(CHANNELLEN));
        buf.append(" CHANTYPES=");
        buf.append(CHANTYPES);
        buf.append(" ELIST=");
        buf.append(ELIST);
        buf.append(" EXCEPTS=");
        buf.append(EXCEPTS);
        buf.append(" EXTBAN=");
        buf.append(EXTBAN);
        buf.append(" HOSTLEN=" + std::to_string(HOSTLEN));
        buf.append(" INVEX=");
        buf.append(INVEX);
        buf.append(" KICKLEN=" + std::to_string(KICKLEN));
        buf.append(" MAXLIST=");
        buf.append(MAXLIST);
        buf.append(" MAXTARGET=" + std::to_string(MAXTARGET));
        buf.append(" MODES=");
        buf.append(MODES);
        buf.append(CRLF);
        append(sender->getSendBuffer(), buf);
        sender->isupportTokenPackCount = 1;
    }
    else if (sender->isupportTokenPackCount == 1)
    {
        std::string buf;
        buf.append(" NETWORK=");
        buf.append(NETWORK);
        buf.append(" NICKLEN=" + std::to_string(NICKLEN));
        buf.append(" PREFIX=");
        buf.append(PREFIX);
        buf.append(" SAFELIST=");
        buf.append(SAFELIST);
        buf.append(" SILENCE=" + std::to_string(SILENCE));
        buf.append(" STATUSMSG=");
        buf.append(STATUSMSG);
        buf.append(" TARGMAX=");
        buf.append(TARGMAX);
        buf.append(" TOPICLEN=" + std::to_string(TOPICLEN));
        buf.append(" USERLEN=" + std::to_string(USERLEN));
        buf.append(CRLF);
        append(sender->getSendBuffer(), buf);
        sender->isupportTokenPackCount = 2;
    }
}

void CommandManager::doCommands(std::deque<Message> &msgs, User *sender)
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
        else if (sender->isRegistered() && msgs.front().command.compare("PING") == 0)
            sender->send(ping(msgs.front(), sender));
        else if (sender->isRegistered() && msgs.front().command.compare("JOIN") == 0)
            sender->send(join(msgs.front(), sender, cManager_));
        else
            sender->send(ERR_UNKNOWNCOMMAND(sender->nick, msgs.front().command));
        msgs.pop_front();
        if (!sender->isRegistered() && sender->isPassSent() && !sender->nick.empty() && !sender->username.empty())
        {
            sender->nickmask = sender->nick + "!" + sender->username + "@localhost";
            sender->setRegistered(true);
            sender->send(RPL_WELCOME(sender->username, sender->nickmask));
        }
    }
    if (sender->isupportTokenPackCount != 2)
        sendIsupport(sender);
}
