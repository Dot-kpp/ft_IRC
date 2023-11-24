#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "Command.hpp"
#include "Nick.hpp"
#include "Ping.hpp"
#include <string>
#include <map>
#include <memory>

class Nick;
class Command;

class CommandHandler
{
private:
    std::map<std::string, std::unique_ptr<Command> > commands;

public:
    CommandHandler();

    bool handleCommand(const std::string &commandName, std::string buffer, int clientFd);
};

#endif // COMMANDHANDLER_HPP
