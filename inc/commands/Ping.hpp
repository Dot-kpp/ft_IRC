
#ifndef PING_HPP
#define PING_HPP

#include "Command.hpp"

class Ping : public Command
{
private:
    std::string command;

public:
    Ping();
    virtual ~Ping();
    
    virtual bool execute(Server *server, std::string args, int clientFd);
};

#endif