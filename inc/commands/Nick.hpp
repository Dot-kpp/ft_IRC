
#ifndef NICK_HPP
#define NICK_HPP

#include "Command.hpp"

class Nick : public Command
{
private:
    std::string command;

public:
    Nick() : command("NICK") {};
    virtual ~Nick();
    
    virtual bool execute(Server *server, std::string args, int clientFd);
    bool parseNickname(std::string nickname, int clientFd);
};

#endif