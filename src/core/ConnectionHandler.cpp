#include "ConnectionHandler.h"
#include <cstdlib>
#include <sstream>
#include <sys/socket.h>

ConnectionHandler::ConnectionHandler()
{
}

ConnectionHandler::~ConnectionHandler()
{
}

void ConnectionHandler::setLogger(Logger &logger)
{
    logger_ = &logger;
}

bool ConnectionHandler::sendData(int fd, std::string const &data)
{
    const char *buffer = data.c_str();
    size_t totalSent = 0;
    size_t bytesLeft = data.size();
    ssize_t bytesSent = 0;

    bytesSent = send(fd, buffer + totalSent, bytesLeft - totalSent, MSG_NOSIGNAL);
    if (bytesSent == -1)
    {
        logger_->log("send: " + std::string(strerror(errno)), Logger::ERROR);
        return false;
    }
    return true;
}

std::string ConnectionHandler::recvData(int fd)
{
    std::string data;
    std::vector<char> buffer(1024);
    ssize_t bytesRead = 0;
    bytesRead = recv(fd, buffer.data(), buffer.size(), MSG_DONTWAIT);
    if (bytesRead > 0)
        data.append(buffer.data(), bytesRead);
    else if (bytesRead <= 0)
    {
        if (bytesRead == 0)
            logger_->log("connection closed", Logger::INFO);
        else
            logger_->log("recv: " + std::string(strerror(errno)), Logger::ERROR);
        close(fd);
    }
    return data;
}
