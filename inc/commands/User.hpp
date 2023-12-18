
#ifndef USER_HPP
#define USER_HPP

#include "Command.hpp"

class User : public Command
{
private:
    std::string command;

public:
    User();
    virtual ~User();
    
    virtual bool execute(Server *server, std::string args, int clientFd);
};

#endif // NICK_HPP