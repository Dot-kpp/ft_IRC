
#ifndef QUIT_HPP
#define QUIT_HPP

#include "Command.hpp"

class Quit : public Command
{
private:
    std::string command;
public:
    Quit();
    virtual ~Quit();
    
    virtual bool execute(Server *server, std::string args, int clientFd);
};

#endif