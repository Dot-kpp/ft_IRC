
#include "../../inc/commands/Quit.hpp"

Quit::Quit() : command("QUIT") {};

Quit::~Quit() {};

void    tellEveryoneButSender(Server *server, std::string message, int clientFd)
{
    for (std::map<int, Client>::iterator it = server->clients.begin(); it != server->clients.end(); it++)
    {
        if (it->first != clientFd) {
            send(it->first, message.c_str(), message.size(), 0);
        }
    }
}

bool Quit::execute(Server *server, std::string args, int clientFd)
{
    std::string serverName = "YourServerName";

    std::string message = args;
    if (message.empty())
        message = "Bye for now!";
    std::stringstream ss;
    ss << ":" << serverName << " " << server->clients[clientFd].getNickName() << " is exiting the network with the message: Quit: " << message << "\r\n";
    tellEveryoneButSender(server, ss.str(), clientFd);
    server->removeClient(clientFd);
    return true;
}