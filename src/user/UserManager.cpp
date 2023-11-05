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

void UserManager::create(int fd)
{
    User *newUser = new User(fd);
    users_[fd] = newUser;
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
