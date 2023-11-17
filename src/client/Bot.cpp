#include "Bot.h"

Bot::Bot() : Client()
{
    nick = BOT_NAME;
    username = BOT_NAME;
    realname = "ROBOT";
    updateNickmask();
}

Bot::~Bot()
{
}
