#pragma once

#include "../message/Message.h"
#include <iostream>
#include <sstream>
#include <string>

class Buffer
{
  public:
    static void treatBuffer(std::string &recvBuffer, std::string &sendBuffer);

  private:
    Buffer();
    ~Buffer();
};
