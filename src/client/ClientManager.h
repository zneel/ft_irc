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
    typedef std::map<int, Client *>::iterator UserMapIterator;
    typedef std::vector<epoll_event>::iterator EventsIterator;

    ClientManager();
    ~ClientManager();
    int getUserCount();
    std::map<int, Client *> &getUsers();
    Client *get(int fd);

    /**
     * @brief Creates a new user.
     *
     */
    Client *create(int fd, std::string ip, IObserver *observer);

    /**
     * @brief Removes a user from the user manager.
     *
     * @param fd The file descriptor of the user to remove.
     */
    void remove(int fd);

    /**
     * @brief Removes all users from the user manager.
     */
    void removeAll();

    bool nickAlreadyUsed(std::string const &nick);

  private:
    ClientManager &operator=(ClientManager const &rhs);
    ClientManager(ClientManager const &other);
    std::map<int, Client *> users_;
};
