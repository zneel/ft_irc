#include "Buffer.h"

void Buffer::swapBuffer(std::string &buffer1, std::string &buffer2)
{
    std::string tmp = buffer1;
    buffer1 = buffer2;
    buffer2 = tmp;
}

void Buffer::treatBuffer(std::string &recvBuffer, std::string &sendBuffer)
{
    (void)recvBuffer;
    (void)sendBuffer;
}
