#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "../user/User.h"
#include "../user/UserManager.h"

#define MAX_LISTENER 10

class Server
{
    typedef std::vector<pollfd>::iterator PfdIterator;

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
    void errorExit_(std::string const &msg) const;

    /**
     * @brief Adds a file descriptor to the list of file descriptors to poll.
     *
     * @param fd The file descriptor to add.
     */
    void addClient_(int fd);

    /**
     * Returns a pointer to the address portion of the given sockaddr struct.
     *
     * @param sa A pointer to a sockaddr struct.
     * @return A pointer to the address portion of the given sockaddr struct.
     */
    void *getInAddr_(struct sockaddr *sa);

    /**
     * Listens for incoming connections on the server socket.
     * @return The file descriptor of the accepted connection.
     */
    int listenForConn_();

    /**
     * @brief Accepts a new incoming connection.
     *
     * @return int The file descriptor of the accepted connection.
     */
    void acceptConn_();

    /**
     * @brief Handles a client connection.
     *
     */
    void handleClient_(struct pollfd pfd, int i);

    /**
     * @brief Disconnects a client with the given file descriptor.
     *
     * @param fd The file descriptor of the client to disconnect.
     */
    void disconnectClient_(int fd, int i);

    std::string const port_;
    std::string const password_;
    char buff_[60];

    UserManager uManager_;
    std::vector<pollfd> clients_;

    bool run_;
    int listener_;
};
