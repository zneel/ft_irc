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
	std::vector<Client *> privmsg = user->getPrivmsg();
    for (std::vector<Client *>::iterator it = privmsg.begin(); it != privmsg.end(); it++)
    {

        (*it)->send(":" + user->nickmask + " QUIT " + msg.trailling);
		(*it)->removePrivmsg(user->nick);
		user->removePrivmsg((*it)->nick);
    }
	user->setShouldDisconnect(true);
	return error(msg, user);
}