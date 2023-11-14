#include "Client.h"

Client::Client(int fd, std::string ip, IObserver *observer)
    : nick(""), username(""), realname(""), ip(ip), fd_(fd), shouldDisconnect_(false), registered_(false),
      passSent_(false)
{
    observer_ = observer;
}

Client::~Client()
{
    disconnect();
}

int Client::getFd() const
{
    return fd_;
}

std::string &Client::getSendBuffer()
{
    return sendBuffer_;
}

std::string &Client::getRecvBuffer()
{
    return recvBuffer_;
}

void Client::setSendBuffer(std::string const &sendBuffer)
{
    sendBuffer_ = sendBuffer;
}

void Client::setRecvBuffer(std::string const &recvBuffer)
{
    recvBuffer_ = recvBuffer;
}

void Client::sendMany(std::vector<std::string> messages)
{
    for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it)
        send(*it);
}

Client::RolePrefix Client::getRoleInChannel(std::string const &channelName)
{
    return channelRoles_[channelName];
}

void Client::setRoleInChannel(std::string const &channelName, RolePrefix role)
{
    channelRoles_[channelName] = role;
}

std::string Client::RolePrefixToString(RolePrefix role)
{
    if (role == OPERATOR)
        return "@";
    else if (role == VOICE)
        return "+";
    else
        return "";
}

void Client::send(std::string message)
{
    if (message.empty())
        return;
    if (message.length() > MAX_SIZE_SEND_BUFFER)
        message.erase(MAX_SIZE_SEND_BUFFER, std::string::npos);
    sendBuffer_.append(message + CRLF);
    notifyObserver(EPOLLOUT);
}

bool Client::shouldDisconnect() const
{
    return shouldDisconnect_ == true;
}

void Client::setShouldDisconnect(bool shouldDisconnect)
{
    shouldDisconnect_ = shouldDisconnect;
}

bool Client::isRegistered() const
{
    return registered_ == true;
}

void Client::setRegistered(bool registered)
{
    registered_ = registered;
}

void Client::setPassSent(bool passSent)
{
    passSent_ = passSent;
}

void Client::setOp(bool op)
{
    op_ = op;
}

bool Client::isOp() const
{
    return op_;
}

void Client::updateNickmask() 
{
    nickmask = nick + "!" + username + "@localhost";
}

bool Client::isPassSent() const
{
    return passSent_ == true;
}

void Client::disconnect()
{
    close(fd_);
    fd_ = -1;
}

Client::Client(Client const &other) : fd_(other.fd_)
{
}

Client &Client::operator=(Client const &rhs)
{
    if (this != &rhs)
    {
    }
    return *this;
}

bool Client::operator==(Client const &rhs)
{
    return fd_ == rhs.fd_;
}

void Client::notifyObserver(EPOLL_EVENTS event)
{
    if (observer_)
        observer_->update(fd_, event);
}
