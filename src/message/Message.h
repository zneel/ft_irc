#pragma once

#include <string>
#include <sstream>

typedef struct s_Message
{
    std::string command;
    std::string parameters;
}t_Message;

t_Message newMessage(std::string line);