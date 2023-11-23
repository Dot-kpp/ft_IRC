#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../server/Server.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>


class Command
{
public:
    Command() {};
    virtual ~Command();
    virtual bool execute(std::string args, int clientFd) = 0;
};
#endif
