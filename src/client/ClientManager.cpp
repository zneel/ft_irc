#include "ClientManager.h"

ClientManager::ClientManager()
{
}

ClientManager::~ClientManager()
{
    removeAll();
}

int ClientManager::getUserCount()
{
    return users_.size();
}

std::map<int, Client *> &ClientManager::getUsers()
{
    return users_;
}

Client *ClientManager::get(int fd)
{
    UserMapIterator it = users_.find(fd);
    if (it != users_.end())
        return it->second;
    return NULL;
}

Client *ClientManager::create(int fd, std::string ip, IObserver *observer)
{
    Client *newUser = new Client(fd, ip, observer);
    users_[fd] = newUser;
    return users_[fd];
}

void ClientManager::remove(int fd)
{
    UserMapIterator it = users_.find(fd);
    if (it != users_.end())
    {
        delete it->second;
        users_.erase(it);
    }
}

void ClientManager::removeAll()
{
    for (UserMapIterator it = users_.begin(); it != users_.end(); ++it)
        delete it->second;
    users_.clear();
}

bool ClientManager::nickAlreadyUsed(std::string const &nick)
{
    for (UserMapIterator it = users_.begin(); it != users_.end(); ++it)
    {
        if (it->second->nick == nick)
            return true;
    }
    return false;
}
