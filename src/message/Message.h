
#pragma once

#include <string>

class Message
{
  public:
    Message(std::string content);
    virtual ~Message();

  private:
    std::string command_;
    std::string parameters_;
};
