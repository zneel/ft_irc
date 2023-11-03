#pragma once

#include "User.h"
#include <map>

class UserManager
{
  public:
    UserManager();
    ~UserManager();
    int getUserCount();
    /**
     * @brief Creates a new user.
     *
     */
    void create(int fd);

    /**
     * @brief Removes a user from the user manager.
     *
     * @param fd The file descriptor of the user to remove.
     */
    void remove(int fd);

  private:
    UserManager &operator=(UserManager const &rhs);
    UserManager(UserManager const &other);
    std::map<int, User *> users_;
};
