
#include "../../inc/commands/Quit.hpp"

Quit::Quit() : command("QUIT") {};

Quit::~Quit() {};



bool Quit::execute(Server *server, std::string args, int clientFd)
{
    std::string serverName = "YourServerName";

    std::string message = args;
    if (message.empty())
        message = "Bye for now!";
    server->removeClient(clientFd, message);
    return true;
}