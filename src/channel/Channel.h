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
    int maxUser;

    std::map<int, Client *> getUsers();
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

    void addUser(Client *user);
    void removeUser(Client *user);

    void addOperator(Client *user);
    void removeOperator(Client *user);

    std::string modeToString();

    int getUserCount();

    bool isUserBanned(Client *user);
    void addBan(Client *user);
    void removeBan(Client *user);

    bool isUserOnExceptionList(Client *user);
    void addException(Client *user);
    void removeException(Client *user);

    bool isOnInviteList(Client *user);
    void addInvite(Client *user);
    void removeInvite(Client *user);

    void broadcast(std::string const &message, Client *sender, bool sendToSender = false);

  private:
    int mode_;
    int type_;
    std::map<int, Client *> users_;
    std::map<int, Client *> operators_;
    std::map<std::string, Client *> ban_;
    std::map<std::string, Client *> exceptionList_;
    std::map<std::string, Client *> inviteList_;
};
