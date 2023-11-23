
#include "../../inc/commands/CommandHandler.hpp"

CommandHandler::CommandHandler()
{
    commands["NICK"] = std::auto_ptr<Nick>(new Nick());
};

bool CommandHandler::handleCommand(const std::string &commandName, std::string buffer, int clientFd)
{
    if (commands.find(commandName) != commands.end())
    {
        return commands[commandName]->execute(buffer, clientFd);
    }
    return false;
}