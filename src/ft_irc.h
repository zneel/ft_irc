#pragma once
#include <bits/types/sig_atomic_t.h>
#include <csignal>
#include <iostream>
#include <signal.h>

#define RPL_WELCOME(client, nick, host) "001 " x " :Welcome to ft_irc " x "!" x "@" host
#define RPL_YOURHOST(client, servname, vers) "002 " client " :Your host is " servname ", running version " vers
#define RPL_CREATED(client, datetime) "003 " client " :This server was created " datetime
#define RPL_MYINFO(client, servname, vers, umodes, cmodes) "004 " client " " servname " " vers " " umodes " " cmodes
#define RPL_ISUPPORT(client, tokens) "005 " client " " tokens

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
