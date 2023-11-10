#pragma once

#include "../client/Client.h"
#include <map>
#include <string>

class Channel
{

  public:
    // have to handle i, t, k, o, l
    // i = invite only
    // t = topic settable by channel operator only
    // k = key
    // o = channel operator
    // l = limit
    enum Mode
    {
        BAN = (1 << 0),                  // +b
        EXCEPTION = (1 << 1),            // +e
        CLIENT_LIMIT = (1 << 2),         // +l
        INVITE_ONLY = (1 << 3),          // +i
        INVITE_EXCEPTION = (1 << 4),     // +I
        KEY = (1 << 5),                  // +k
        MODERATED = (1 << 6),            // +m
        SECRET = (1 << 7),               // +s
        PROTECTED_TOPIC = (1 << 8),      // +t
        NO_EXTERNAL_MESSAGES = (1 << 9), // +n
    };

    // only #
    enum Type
    {
        REGULAR = (1 << 0),
        LOCAL = (1 << 1),
    };

    Channel(std::string name, Mode mode, Type type);
    ~Channel();

    std::string name;
    std::string password;
    std::string topic;
    int maxClient;

    std::map<int, Client *> getClients();
    std::map<int, Client *> getOperators();

    int getMode();
    void setMode(int mode);
    void addMode(int mode);
    bool hasMode(int mode);
    bool hasModes(int modes);

    int getType();
    void setType(int type);
    void addType(int type);
    bool hasType(int type);

    void addClient(Client *client);
    void removeClient(Client *client);

    void addOperator(Client *client);
    void removeOperator(Client *client);

    std::string modeToString();

    int getClientCount();

    bool isClientBanned(Client *client);
    void addBan(Client *client);
    void removeBan(Client *client);

    bool isClientOnExceptionList(Client *client);
    void addException(Client *client);
    void removeException(Client *client);

    bool isOnInviteList(Client *client);
    void addInvite(Client *client);
    void removeInvite(Client *client);

    void broadcast(std::string const &message, Client *sender, bool sendToSender = false);

    bool isClientOnChannel(Client *client);

  private:
    int mode_;
    int type_;
    std::map<int, Client *> clients_;
    std::map<int, Client *> operators_;
    std::map<std::string, Client *> ban_;
    std::map<std::string, Client *> exceptionList_;
    std::map<std::string, Client *> inviteList_;
};
