#pragma once

#include <string>
class Buffer
{
  public:
    void swapBuffer(std::string &buffer1, std::string &buffer2);
    void treatBuffer(std::string &recvBuffer, std::string &sendBuffer);

  private:
    Buffer();
    ~Buffer();
};
