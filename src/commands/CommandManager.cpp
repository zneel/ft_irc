#include "CommandManager.h"
#include "../user/UserManager.h"

CommandManager::CommandManager(ChannelManager *cManager, UserManager *uManager)
{
    cManager_ = cManager;
    uManager_ = uManager;
}

CommandManager::~CommandManager()
{
}

void CommandManager::doCommands(std::deque<Message> &msgs, User *sender)
{
    while (!msgs.empty())
    {
        if (msgs.front().command.compare("CAP") == 0)
        {
			append(sender->getSendBuffer(), cap(msgs.front(), sender));
        }
        msgs.pop_front();
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
