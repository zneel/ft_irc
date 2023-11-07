#include "ConnectionHandler.h"
#include <cstdlib>
#include <iterator>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>

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

ssize_t ConnectionHandler::recvData(int fd, std::string &buffer)
{
    char tmpBuff[BUFFER_SIZE];
    ssize_t bytesRead = recv(fd, tmpBuff, sizeof(tmpBuff), 0);
    if (bytesRead < 0)
    {
        logger_->log("recv: " + std::string(strerror(errno)), Logger::ERROR);
        return bytesRead;
    }
    buffer.append(tmpBuff, bytesRead);
    return bytesRead;
}

ssize_t ConnectionHandler::sendData(int fd, std::string &message)
{
    ssize_t bytesSent = send(fd, message.data(), message.size(), 0);
    if (bytesSent == -1)
    {
        logger_->log("send: " + std::string(strerror(errno)), Logger::ERROR);
        return bytesSent;
    }
    return bytesSent;
}
