#include "../ft_irc.h"
#include <string>

std::string const RPL_WELCOME(std::string const &client, std::string const &nickmask)
{
    return "001 " + client + " :Welcome to the Internet Relay Network " + nickmask;
}

std::string const RPL_YOURHOST(std::string const &client, std::string const &servname, std::string const &vers)
{
    return "002 " + client + " :Your host is " + servname + ", running version " + vers;
}

std::string const RPL_CREATED(std::string const &client, std::string const &datetime)
{
    return "003 " + client + " :This server was created " + datetime;
}

std::string const RPL_MYINFO(std::string const &client, std::string const &sname, std::string const &vers,
                             std::string const &umodes, std::string const &cmodes)
{
    return "004 " + client + " " + sname + " " + vers + " " + umodes + " " + cmodes;
}

std::string const RPL_ISUPPORT(std::string const &client, std::string const &tokens)
{
    return "005 " + client + tokens;
}

////
// CHANNELS
////

std::string const RPL_TOPICWHOTIME(std::string const &client, std::string const &channel, std::string const &nick,
                                   std::string const &time)
{
    return "333 " + client + " " + channel + " " + nick + " " + time;
}

std::string const RPL_NAMREPLY(std::string const &client, std::string const &channel, std::string const &nicks)
{
    return "353 " + client + " = " + channel + " :" + nicks;
}

std::string const RPL_ENDOFNAMES(std::string const &client, std::string const &channel)
{
    return "366 " + client + " " + channel + " :End of /NAMES list";
}

std::string const RPL_YOUREOPER(std::string const &client)
{
    return "381 " + client + " :You are now an IRC operator";
}
