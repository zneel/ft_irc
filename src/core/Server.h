#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
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

#include "../ft_irc.h"
#include "../user/User.h"
#include "../user/UserManager.h"
#include "ConnectionHandler.h"
#include "Logger.h"

#define MAX_LISTENER 10

class Server
{
    typedef std::vector<pollfd>::iterator PollFdsIterator;

  public:
    Server(std::string port, std::string password);
    ~Server();

    /**
     * @brief Starts the server.
     *
     */
    void start();

    /**
     * @brief Sets the logger for the server.
     *
     * @param logger The logger to set.
     */
    void setLogger(Logger &logger);

  private:
    /**
     * @brief Adds a file descriptor to the list of file descriptors to poll.
     *
     * @param fd The file descriptor to add.
     */
    void connectAndAddToPolling(int fd);

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
    int listenForConnection();

    /**
     * @brief Accepts a new incoming connection.
     *
     * @return int The file descriptor of the accepted connection.
     */
    void acceptConnection();

    /**
     * @brief Handles a client connection.
     *
     */
    void handleClient(struct pollfd pfd, int i);

    /**
     * @brief Disconnects a client with the given file descriptor.
     *
     * @param fd The file descriptor of the client to disconnect.
     */
    void removeFromPolling(int fd, int i);

    void initListener();

    std::string const port_;
    std::string const password_;
    Logger logger_;

    UserManager uManager_;

    std::vector<pollfd> pollFds_;
    ConnectionHandler connectionHandler_;

    int listener_;
};
