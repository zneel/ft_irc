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

bool ConnectionHandler::sendData(int fd, ClientBuffer &cb)
{
    ssize_t bytesSent = send(fd, cb.buffer.data() + cb.bytesSent, cb.buffer.size() - cb.bytesSent, MSG_NOSIGNAL);
    if (bytesSent == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return true;
        logger_->log("send: " + std::string(strerror(errno)), Logger::ERROR);
        return false;
    }
    if (bytesSent < static_cast<ssize_t>(cb.buffer.size()))
        cb.bytesSent += bytesSent;
    logger_->log("sent: " + std::string(cb.buffer.begin(), cb.buffer.end()), Logger::INFO);
    return true;
}

bool ConnectionHandler::recvData(int fd, ClientBuffer &cb)
{
    std::vector<char> tmpBuff(1024, 0);
    ssize_t bytesRead = recv(fd, tmpBuff.data(), tmpBuff.size(), MSG_DONTWAIT);
    if (bytesRead <= 0)
    {
        if (bytesRead == 0)
            logger_->log("connection closed", Logger::INFO);
        else
            logger_->log("recv: " + std::string(strerror(errno)), Logger::ERROR);
        close(fd);
        return false;
    }
    cb.buffer.insert(cb.buffer.end(), tmpBuff.begin(), tmpBuff.begin() + bytesRead);
    return true;
}
