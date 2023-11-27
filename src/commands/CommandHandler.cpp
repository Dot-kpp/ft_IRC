
#include "../../inc/commands/CommandHandler.hpp"

CommandHandler::CommandHandler()
{
    commands["NICK"] = std::auto_ptr<Nick>(new Nick());
    commands["PING"] = std::auto_ptr<Ping>(new Ping());
    commands["USER"] = std::auto_ptr<User>(new User());
};

bool CommandHandler::handleCommand(const std::string &commandName, std::string buffer, int clientFd)
{
    Server *server = Server::instance;
    buffer.erase(0, commandName.size() + 1);
    if (commands.find(commandName) != commands.end())
    {
        bool returnValue = commands[commandName]->execute(server, buffer, clientFd);
        std::cout << "YOO" << std::endl;
        if (!server->clients[clientFd].getIsRegistered() && (!server->clients[clientFd].getNickName().empty() && !server->clients[clientFd].getUserName().empty()))
            server->clients[clientFd].welcomeClient(clientFd);
        return returnValue;
    }
    return false;
}

bool CommandHandler::isCommandRegistered(const std::string &commandName)
{
    return commands.find(commandName) != commands.end();
}