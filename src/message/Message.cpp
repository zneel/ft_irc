#include "Message.h"
#include <iostream>

Message newMessage(std::string line)
{
    Message msg;
    std::string::iterator begin = line.begin();
    std::string::iterator end = line.end();
    size_t tmp = line.find(" ");
    if (tmp != std::string::npos)
    {
        std::string::iterator mid = line.begin() + tmp;
        msg.command.append(std::string(begin, mid));
        ++mid;
        msg.parameters.append(std::string(mid, end));
    }
    else
        msg.command.append(begin, end);
    return msg;
}
