#include "UserManager.h"
#include <utility>

UserManager::UserManager()
{
}

UserManager::~UserManager()
{
    for (std::map<int, User *>::iterator it = users_.begin(); it != users_.end(); ++it)
        delete it->second;
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
    std::map<int, User *>::iterator it = users_.find(fd);
    if (it != users_.end())
    {
        delete it->second;
        users_.erase(it);
    }
}
