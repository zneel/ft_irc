#pragma once

#include <sstream>
#include <string>

typedef struct s_Message
{
    std::string command;
    std::string parameters;
} t_Message;

t_Message newMessage(std::string line);