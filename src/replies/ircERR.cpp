#include "../ft_irc.h"
#include <string>

std::string const ERR_PASSWDMISMATCH(std::string const &client)
{
    return "464 " + client + " :Password incorrect";
}

std::string const ERR_ALREADYREGISTERED(std::string const &client)
{
    return "462 " + client + " :You may not reregister";
}

std::string const ERR_NEEDMOREPARAMS(std::string const &client, std::string const &cmd)
{
    return "461 " + client + " " + cmd + " :Not enough parameters";
}

std::string const ERR_NICKNAMEINUSE(std::string const &client, std::string const &nick)
{
    return "433 " + client + " " + nick + " :Nickname is already in use";
}

std::string const ERR_NONICKNAMEGIVEN(std::string const &client)
{
    return "431 " + client + " :No nickname given";
}

std::string const ERR_ERRONEUSNICKNAME(std::string const &client, std::string const &nick)
{
    return "432 " + client + " " + nick + " :Erroneus nickname";
}

std::string const ERR_ERROR(std::string const &reason)
{
    return "ERROR :" + reason;
}

std::string const ERR_UNKNOWNCOMMAND(std::string const &client, std::string const &command)
{
    return "421 " + client + " " + command + " :Unknown command";
}
