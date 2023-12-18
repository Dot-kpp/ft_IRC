#include "../../inc/commands/Ping.hpp"

Ping::Ping() : command("Ping") {};

Ping::~Ping()
{
}

bool Ping::execute(Server *server,std::string args, int clientFd)
{
    (void)server;
    std::string token = args.substr(0, args.find(" "));

    if (token.empty())
    {
        std::stringstream ss;
        ss << ":" << Server::instance->getServerName() << " 409 " << Server::instance->clients[clientFd].getNickName() << " "
           << " :No origin specified\r\n";
        std::string noOriginMsg = ss.str();
        send(clientFd, noOriginMsg.c_str(), noOriginMsg.size(), 0);
        return false;
    }
    else {
        std::stringstream ss;
        ss << ":" << Server::instance->getServerName() << " PONG " << Server::instance->getServerName() << " " << token << "\r\n";
        std::string pongMsg = ss.str();
        send(clientFd, pongMsg.c_str(), pongMsg.size(), 0);
    }
    return true;
}
