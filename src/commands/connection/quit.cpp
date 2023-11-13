#include "../CommandManager.h"

std::string quit(Message &msg, Client *user, ChannelManager *chanManager)
{
	std::map<std::string, Channel *> channels = chanManager->getAll();
	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (it->second->isClientOnChannel(user))
		{
			it->second->broadcast(":" + user->nickmask + " QUIT " + msg.trailling, user);
		}
	}
	user->setShouldDisconnect(true);
	return error(msg, user);
}