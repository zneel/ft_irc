#pragma once

#include <map>
#include <sys/epoll.h>
#include <vector>

#include <iostream>
#include <utility>

#include "Client.h"

class ClientManager
{

  public:
    typedef std::map<int, Client *>::iterator ClientMapIterator;
    typedef std::vector<epoll_event>::iterator EventsIterator;

    ClientManager();
    ~ClientManager();
    int getClientCount();
    std::map<int, Client *> &getClients();
    Client *get(int fd);

    /**
     * @brief Creates a new client.
     *
     */
    Client *create(int fd, std::string ip, IObserver *observer);

    /**
     * @brief Removes a client from the client manager.
     *
     * @param fd The file descriptor of the client to remove.
     */
    void remove(int fd);

    /**
     * @brief Removes all clients from the client manager.
     */
    void removeAll();

    bool nickAlreadyUsed(std::string const &nick);

  private:
    ClientManager &operator=(ClientManager const &rhs);
    ClientManager(ClientManager const &other);
    std::map<int, Client *> clients_;
};
