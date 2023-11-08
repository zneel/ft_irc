#pragma once

#include "../user/User.h"
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

    std::map<int, User *> getUsers();
    std::map<int, User *> getOperators();

    int getMode();
    void setMode(int mode);
    void addMode(int mode);
    bool hasMode(int mode);

    int getType();
    void setType(int type);
    void addType(int type);
    bool hasType(int type);

    void addUser(User *user);
    void removeUser(User *user);

    void addOperator(User *user);
    void removeOperator(User *user);

    std::string modeToString();

    int getUserCount();

    bool isUserBanned(User *user);
    void addBan(User *user);
    void removeBan(User *user);

    bool isUserOnExceptionList(User *user);
    void addException(User *user);
    void removeException(User *user);

    bool isOnInviteList(User *user);
    void addInvite(User *user);
    void removeInvite(User *user);

  private:
    int mode_;
    int type_;
    std::map<int, User *> users_;
    std::map<int, User *> operators_;
    std::map<std::string, User *> ban_;
    std::map<std::string, User *> exceptionList_;
    std::map<std::string, User *> inviteList_;
};
