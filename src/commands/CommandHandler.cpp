
#include "../../inc/commands/CommandHandler.hpp"

CommandHandler::CommandHandler()
{
    commands["NICK"] = std::auto_ptr<Nick>(new Nick());
    commands["PING"] = std::auto_ptr<Ping>(new Ping());
};

bool CommandHandler::handleCommand(const std::string &commandName, std::string buffer, int clientFd)
{
    buffer.erase(0, commandName.size() + 1);
    if (commands.find(commandName) != commands.end())
    {
        return commands[commandName]->execute(buffer, clientFd);
    }
    return false;
}