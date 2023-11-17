#include "Bot.h"

Bot::Bot() : Client()
{
    nick = "BOT";
    username = "BOT";
    realname = "ROBOT";
    updateNickmask();
}

Bot::~Bot()
{
}
