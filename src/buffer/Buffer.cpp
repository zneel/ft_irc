#include "Buffer.h"

int howManyCRLF(std::string &buffer)
{
    int nbCRLF = 0;
    for (std::string::iterator it = buffer.begin(); it != buffer.end() - 1; ++it)
    {
        if (*it == '\r' && *(it + 1) == '\n')
            nbCRLF++;
    }

    return nbCRLF;
}

void Buffer::treatBuffer(std::string &recvBuffer, std::string &sendBuffer)
{
    int nbCRLF = howManyCRLF(recvBuffer);
    for (int i = 0; i < nbCRLF; i++)
    {
        std::string::iterator nextCRLF = recvBuffer.begin() + recvBuffer.find("\r\n");
        if (nextCRLF != recvBuffer.begin())
        {
            std::string::iterator begin = recvBuffer.begin();
            Message msg = newMessage(std::string(begin, nextCRLF));
            sendBuffer.append("return value of exec");
            recvBuffer.erase(0, *nextCRLF);
        }
    }
}
