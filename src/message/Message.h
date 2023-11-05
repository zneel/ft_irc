#pragma once

#include <string>
class Message
{
  public:
    Message(std::string &message);
    ~Message();
    void printMessage();

  private:
    std::string message_;
};
