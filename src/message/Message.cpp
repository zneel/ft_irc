#include "Message.h"
#include <iostream>

t_Message newMessage(std::string line)
{
	t_Message msg;
	std::string::iterator begin = line.begin();
	std::string::iterator end = line.end();
	size_t tmp = line.find(" ");
	std::string::iterator mid = line.begin() + tmp * (tmp != std::string::npos);
	msg.command.append(std::string (begin, mid));
	msg.parameters.append(std::string (mid, end));
	return msg;
}
