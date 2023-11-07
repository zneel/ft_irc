#pragma once
#include <bits/types/sig_atomic_t.h>
#include <csignal>
#include <iostream>
#include <signal.h>

#define MAX_SIZE_SEND_BUFFER 510

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
std::string const RPL_WELCOME(std::string const &client, std::string const &nick, std::string const &host);
std::string const RPL_YOURHOST(std::string const &client, std::string const &servname, std::string const &vers);
std::string const RPL_CREATED(std::string const &client, std::string const &datetime);
std::string const RPL_MYINFO(std::string const &client, std::string const &sname, std::string const &vers,
                             std::string const &umodes, std::string const &cmodes);
std::string const RPL_ISUPPORT(std::string const &client, std::string const &tokens);

// ERR
std::string const ERR_PASSWDMISMATCH(std::string const &client);
std::string const ERR_ALREADYREGISTERED(std::string const &client);
std::string const ERR_NEEDMOREPARAMS(std::string const &client, std::string const &cmd);
std::string const ERR_NICKNAMEINUSE(std::string const &client, std::string const &nick);
std::string const ERR_NONICKNAMEGIVEN(std::string const &client);
std::string const ERR_ERRONEUSNICKNAME(std::string const &client, std::string const &nick);
std::string const ERR_ERROR(std::string const &reason);
std::string const ERR_UNKNOWNCOMMAND(std::string const &client, std::string const &command);
