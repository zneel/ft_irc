#pragma once

#include <sstream>
#include <string>

struct Message
{
    std::string command;
    std::string parameters;
};

Message newMessage(std::string line);
