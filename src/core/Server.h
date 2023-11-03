#pragma once

#include "../user/User.h"
#include "../user/UserManager.h"

#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

class Server
{
  public:
    Server(std::string port, std::string password);
    ~Server();

    /**
     * @brief Starts the server.
     *
     */
    void start();

    /**
     * @brief Stops the server and closes all connections.
     */
    void stop();

  private:
    Server(Server const &other);
    Server &operator=(Server const &rhs);

    /**
     * @brief Exits the program with an error message.
     *
     * @param msg The error message to display.
     */
    void errorExit(std::string const &msg) const;

    /**
     * @brief Adds a file descriptor to the list of file descriptors to poll.
     *
     * @param fd The file descriptor to add.
     */
    void addToPollFds(int fd);

    /**
     * Returns a pointer to the address portion of the given sockaddr struct.
     *
     * @param sa A pointer to a sockaddr struct.
     * @return A pointer to the address portion of the given sockaddr struct.
     */
    void *getInAddr(struct sockaddr *sa);

    /**
     * Listens for incoming connections on the server socket.
     * @return The file descriptor of the accepted connection.
     */
    int listenForConn();

    std::string const port_;
    std::string const password_;
    char buff_[60];

    UserManager uManager_;
    std::vector<pollfd> pfds_;

    bool run_;
    int listener_;
};
