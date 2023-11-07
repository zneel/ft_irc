#include "UserManager.h"

#include <iostream>
#include <utility>

UserManager::UserManager()
{
}

UserManager::~UserManager()
{
    for (UserMapIterator it = users_.begin(); it != users_.end(); ++it)
        remove(it->first);
    users_.clear();
}

int UserManager::getUserCount()
{
    return users_.size();
}

std::map<int, User *> &UserManager::getUsers()
{
    return users_;
}

User *UserManager::get(int fd)
{
    UserMapIterator it = users_.find(fd);
    if (it != users_.end())
        return it->second;
    return NULL;
}

User *UserManager::create(int fd, std::string ip)
{
    User *newUser = new User(fd, ip);
    users_[fd] = newUser;
    return users_[fd];
}

void UserManager::remove(int fd)
{
    UserMapIterator it = users_.find(fd);
    if (it != users_.end())
    {
        delete it->second;
        users_.erase(it);
    }
}

void UserManager::removeAll()
{
    for (UserMapIterator it = users_.begin(); it != users_.end(); ++it)
        delete it->second;
    users_.clear();
}

bool UserManager::nickAlreadyUsed(std::string const &nick)
{
    for (UserMapIterator it = users_.begin(); it != users_.end(); ++it)
    {
        if (it->second->nick == nick)
            return true;
    }
    return false;
}
