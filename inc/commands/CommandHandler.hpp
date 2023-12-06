#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "Command.hpp"
#include "Nick.hpp"
#include "User.hpp"
#include "Ping.hpp"
#include "Oper.hpp"
#include "Names.hpp"
#include "Topic.hpp"
#include "Quit.hpp"
#include "Invite.hpp"
#include "Mode.hpp"
#include "Join.hpp"
#include "Part.hpp"
#include "Kick.hpp"
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
    bool isCommandRegistered(const std::string &commandName);
};

#endif
