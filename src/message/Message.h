#pragma once

#include <sstream>
#include <string>

struct Message
{
    Message(std::string line);
    std::string command;
    std::string parameters;
};
