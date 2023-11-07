#include "../CommandManager.h"

bool isErrChar(std::string nickname)
{
	for (std::string::iterator it = nickname.begin(); it != nickname.end(); it++) 
	{
		if (*it == ' ' || *it == ':' || (it == nickname.begin() && *it == '#'))
			return true;
	}
    return false;
}

void nick(Message msg, User *user)
{
    if (msg.parameters.empty())
        user->getSendBuffer().append(ERR_NONICKNAMEGIVEN(""));
    // IF nickname already used
    else if (1)
        user->getSendBuffer().append(ERR_NICKNAMEINUSE("", msg.parameters));
    else if (isErrChar(msg.parameters))
        user->getSendBuffer().append(ERR_ERRONEUSNICKNAME("", ""));
    else
        user->setNick(msg.parameters);
}