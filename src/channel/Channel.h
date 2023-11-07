#pragma once

#include "../user/User.h"
#include <map>

class Channel
{

    // have to handle i, t, k, o, l
    // i = invite only
    // t = topic settable by channel operator only
    // k = key
    // o = channel operator
    // l = limit
    enum Mode
    {
        INVITE_ONLY = (1 << 0),
        PROTECTED_TOPIC = (1 << 1),
        KEY = (1 << 2),
        OPERATOR = (1 << 3),
        LIMIT = (1 << 4),
    };

    enum Type
    {
        REGULAR = (1 << 0),
    };

  public:
    Channel(std::string name, int mode, int type);
    ~Channel();

    std::map<int, User *> getUsers();
    std::map<int, User *> getOperators();

    std::string getName() const;
    void setName(std::string name);

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

    int getUserCount();

  private:
    std::string name_;
    int mode_;
    int type_;

    std::map<int, User *> users_;
    std::map<int, User *> operators_;
};
