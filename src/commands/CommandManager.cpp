#include "CommandManager.h"
#include "../user/UserManager.h"

CommandManager::CommandManager(ChannelManager *cManager, UserManager *uManager, std::string const &pwd)
    : cManager_(cManager), uManager_(uManager), pwd_(pwd)
{
}

CommandManager::~CommandManager()
{
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
    {
        if (sender->isupportTokenPackCount == 0)
        {
            std::string buf;
            buf.append(AWAYLEN);
            buf.append(CASEMAPPING);
            buf.append(CHANLIMIT);
            buf.append(CHANMODES);
            buf.append(CHANNELLEN);
            buf.append(CHANTYPES);
            buf.append(ELIST);
            buf.append(EXCEPTS);
            buf.append(EXTBAN);
            buf.append(HOSTLEN);
            buf.append(INVEX);
            buf.append(KICKLEN);
            buf.append(MAXLIST);
            buf.append(MAXTARGET);
            buf.append(MODES);
            append(sender->getSendBuffer(), RPL_ISUPPORT(sender->nick, buf));
        }
        else if (sender->isupportTokenPackCount == 1)
        {
            std::string buf;
            buf.append(NETWORK);
            buf.append(NICKLEN);
            buf.append(PREFIX);
            buf.append(SAFELIST);
            buf.append(SILENCE);
            buf.append(STATUSMSG);
            buf.append(TARGMAX);
            buf.append(TOPICLEN);
            buf.append(USERLEN);
            append(sender->getSendBuffer(), RPL_ISUPPORT(sender->nick, buf));
        }
        sender->isupportTokenPackCount++;
    }
}
