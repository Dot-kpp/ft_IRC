
#include "../../inc/commands/CommandHandler.hpp"

CommandHandler::CommandHandler()
{
    commands["NICK"] = std::auto_ptr<Nick>(new Nick());
    commands["PING"] = std::auto_ptr<Ping>(new Ping());
    commands["USER"] = std::auto_ptr<User>(new User());
    commands["QUIT"] = std::auto_ptr<Quit>(new Quit());
    commands["NAMES"] = std::auto_ptr<Names>(new Names());
    commands["OPER"] = std::auto_ptr<Oper>(new Oper());
    commands["TOPIC"] = std::auto_ptr<Topic>(new Topic());
};

bool CommandHandler::handleCommand(const std::string &commandName, std::string buffer, int clientFd)
{
    Server *server = Server::instance;
    buffer.erase(0, commandName.size() + 1);
    if (commands.find(commandName) != commands.end())
    {
        bool returnValue = commands[commandName]->execute(server, buffer, clientFd);
        if (!server->clients[clientFd].getIsRegistered() && !server->clients[clientFd].getNickName().empty() && !server->clients[clientFd].getUserName().empty()) {
            server->clients[clientFd].setIsRegistered(true);
            server->welcomeClient(clientFd);
        }
        return returnValue;
    }
    return false;
}

bool CommandHandler::isCommandRegistered(const std::string &commandName)
{
    return commands.find(commandName) != commands.end();
}