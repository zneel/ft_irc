#include "../CommandManager.h"

void pass(Message msg, User *user, std::string password)
{
	if (msg.parameters.compare(password)) {
		user->getSendBuffer().append(ERR_PASSWDMISMATCH(""));
		// close connexion
	}
}