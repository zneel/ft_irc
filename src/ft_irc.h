#pragma once
#include <bits/types/sig_atomic_t.h>
#include <csignal>
#include <iostream>
#include <signal.h>

#define MAX_SIZE_SEND_BUFFER 510
#define MAX_SIZE_NICK 31
#define CHANNEL_MAX_USER 20
#define CHANNEL_MAX_KEY_SIZE 50

#define NB_ISUPPORT_TOKEN_PACK 2
// TOKEN PACK 1
#define AWAYLEN " AWAYLEN=200"
#define CASEMAPPING " CASEMAPPING=ascii"
#define CHANLIMIT " CHANLIMIT=#:50"
#define CHANMODES " CHANMODES="
#define CHANNELLEN " CHANNELLEN=32"
#define CHANTYPES " CHANTYPES=#"
#define ELIST " ELIST=T" // C for creation time | M for mask | N for !mask | T for topic | U for user count
#define EXCEPTS " EXCEPTS="
#define EXTBAN " EXTBAN="
#define HOSTLEN " HOSTLEN=64"
#define INVEX " INVEX="
#define KICKLEN " KICKLEN=200"
#define MAXLIST " MAXLIST="
#define MAXTARGET " MAXTARGET=4"
#define MODES " MODES="
// TOKEN PACK 2
#define NETWORK " NETWORK=ft_irc"
#define NICKLEN " NICKLEN=31" // same len as MAX_SIZE_NICK
#define PREFIX " PREFIX="
#define SAFELIST " SAFELIST"
#define SILENCE " SILENCE=15"
#define STATUSMSG " STATUSMSG=@+"
#define TARGMAX " TARGMAX=KICK:1,LIST:1,PRIVMSG:4,WHOIS:1"
#define TOPICLEN " TOPICLEN=300"
#define USERLEN " USERLEN=18"

#define CRLF "\r\n"

#define LINE1 " ________  _________         _____  _______      ______  \n"
#define LINE2 "|_   __  ||  _   _  |       |_   _||_   __ \\   .' ___  | \n"
#define LINE3 "  | |_ \\_||_/ | | \\_|         | |    | |__) | / .'   \\_| \n"
#define LINE4 "  |  _|       | |             | |    |  __ /  | |        \n"
#define LINE5 " _| |_       _| |_  _______  _| |_  _| |  \\ \\_\\ `.___.'\\ \n"
#define LINE6 "|_____|     |_____||_______||_____||____| |___|`.____ .' \n"
#define LINE7 "                                   by mhoyer & ebouvier\n"
#define LINE8 "Type /join #<channel> to join a channel\n"
#define LINE9 "Type /nick <nickname> to change your nickname\n"
#define LINE10 "Type /quit to quit the server\n"
#define LINE11 "Thank you for using ft_irc!\n"
extern volatile sig_atomic_t stop;
void setupsig();

// RPL
std::string const RPL_WELCOME(std::string const &client, std::string const &nickmask);
std::string const RPL_YOURHOST(std::string const &client, std::string const &servname, std::string const &vers);
std::string const RPL_CREATED(std::string const &client, std::string const &datetime);
std::string const RPL_MYINFO(std::string const &client, std::string const &sname, std::string const &vers,
                             std::string const &umodes, std::string const &cmodes);
std::string const RPL_ISUPPORT(std::string const &client, std::string const &tokens);

// RPL CHANNELS
std::string const RPL_TOPICWHOTIME(std::string const &client, std::string const &channel, std::string const &nick,
                                   std::string const &time);
std::string const RPL_NAMREPLY(std::string const &client, std::string const &channel, std::string const &nicks);
std::string const RPL_ENDOFNAMES(std::string const &client, std::string const &channel);

// ERR
std::string const ERR_PASSWDMISMATCH(std::string const &client);
std::string const ERR_ALREADYREGISTERED(std::string const &client);
std::string const ERR_NEEDMOREPARAMS(std::string const &client, std::string const &cmd);
std::string const ERR_NICKNAMEINUSE(std::string const &client, std::string const &nick);
std::string const ERR_NONICKNAMEGIVEN(std::string const &client);
std::string const ERR_ERRONEUSNICKNAME(std::string const &client, std::string const &nick);
std::string const ERR_ERROR(std::string const &reason);
std::string const ERR_UNKNOWNCOMMAND(std::string const &client, std::string const &command);

// ERR CHANNELS
std::string const ERR_TOOMANYCHANNELS(std::string const &client, std::string const &channel);
std::string const ERR_NOSUCHCHANNEL(std::string const &client, std::string const &channel);
std::string const ERR_BADCHANNELKEY(std::string const &client, std::string const &channel);
std::string const ERR_BANNEDFROMCHAN(std::string const &client, std::string const &channel);
std::string const ERR_CHANNELISFULL(std::string const &client, std::string const &channel);
std::string const ERR_INVITEONLYCHAN(std::string const &client, std::string const &channel);
std::string const ERR_BADCHANMASK(std::string const &client, std::string const &channel);
std::string const ERR_INVALIDMODEPARAM(std::string const &client, std::string const &target, std::string const &mode,
                                       std::string const &parameter, std::string const &reason);
std::string const ERR_INVALIDKEY(std::string const &client, std::string const &channel);
