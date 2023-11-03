#include "Server.h"
#include <cstdlib>
#include <iostream>
#include <sstream>

struct args
{
    int port;
    std::string password;
};

struct args parseArgs(char *port, char *pwd)
{
    struct args res;
    std::stringstream ss;
    ss << port;
    ss >> res.port;
    ss.clear();
    ss.str("");
    ss << pwd;
    res.password = ss.str();
    return res;
}

void printUsageExit()
{
    std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
    ::exit(1);
}

int main(int ac, char **av)
{
    if (ac != 3)
        printUsageExit();
    struct args parsed = parseArgs(av[1], av[2]);
    if (parsed.password.empty() || (parsed.port < 1024 || parsed.port > 65535))
        printUsageExit();
    Server irc(parsed.port, parsed.password);
    return 0;
}
