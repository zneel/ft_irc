#pragma once

#include <map>

#include "User.h"

class UserManager
{

    typedef std::map<int, User *>::iterator UserMapIterator;

  public:
    UserManager();
    ~UserManager();
    int getUserCount();
    std::map<int, User *> &getUsers();
    User *get(int fd);

    /**
     * @brief Creates a new user.
     *
     */
    User *create(int fd, std::string ip);

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
    UserManager &operator=(UserManager const &rhs);
    UserManager(UserManager const &other);
    std::map<int, User *> users_;
};
