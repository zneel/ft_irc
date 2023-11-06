#pragma once

#include <string>
#include <sstream>
#include <iostream>
class Buffer
{
  public:
    static void treatBuffer(std::string &recvBuffer, std::string &sendBuffer);

  private:
    Buffer();
    ~Buffer();
};
