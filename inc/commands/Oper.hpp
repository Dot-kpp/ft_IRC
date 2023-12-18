
#ifndef OPER_HPP
#define OPER_HPP

#include "Command.hpp"
#include <map>
#include <vector>

class Oper : public Command
{
private:
    std::string command;
    std::map<int, std::string> operMap;

public:
    Oper();
    virtual ~Oper();
    
    virtual bool execute(Server *server, std::string args, int clientFd);
};

#endif