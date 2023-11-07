#include "Message.h"
#include <iostream>

Message::Message(std::string line)
{
    if (line.size() > 512)
        line.erase(512, std::string::npos);
    std::string::iterator begin = line.begin();
    std::string::iterator end = line.end();
    size_t tmp = line.find(" ");
    if (tmp != std::string::npos)
    {
        std::string::iterator mid = line.begin() + tmp;
        command.append(std::string(begin, mid));
        ++mid;
        parameters.append(std::string(mid, end));
    }
    else
        command.append(begin, end);
}
