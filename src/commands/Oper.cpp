
#include "../../inc/commands/Oper.hpp"

Oper::Oper() : Command()
{
    this->command = "OPER";
    operMap[1] = "super_admin";
    operMap[2] = "admin";
}

Oper::~Oper()
{
}

bool Oper::execute(Server *server, std::string args, int clientFd)
{
    Client &client = server->clients[clientFd];
    std::istringstream iss(args);

    std::string nickname;
    std::string password;
    iss >> nickname;
    iss >> password;

    if (nickname.empty() || password.empty())
    {
        std::stringstream ss;
        ss << ":" << server->getServerName() << " 461 " << client.getNickName() << " OPER :Not enough parameters\r\n";
        std::string noNickMsg = ss.str();
        send(clientFd, noNickMsg.c_str(), noNickMsg.size(), 0);
        return false;
    }
    std::map<int, Client>::iterator it = server->clients.begin();
    while (it != server->clients.end())
    {
        if (it->second.getNickName() != nickname)
        {
            std::string wrongPassword = ":YourServerName 464 * :Password incorrect. \r\n";
            send(clientFd, wrongPassword.c_str(), wrongPassword.size(), 0);
            return false;
        }
        it++;
    }
    if (password.compare(this->operMap[1]) == 0)
        client.setRoleId(1);
    else if (password.compare(this->operMap[2]) == 0)
        client.setRoleId(2);
    else
    {
        std::stringstream ss;
        ss << ":" << server->getServerName() << " 464 " << client.getNickName() << " :Password incorrect\r\n";
        std::string noNickMsg = ss.str();
        send(clientFd, noNickMsg.c_str(), noNickMsg.size(), 0);
        return false;
    }
    std::stringstream ss;
    ss << ":" << server->getServerName() << " 381 " << client.getNickName() << " :You are now an IRC operator\r\n";
    std::string noNickMsg = ss.str();
    send(clientFd, noNickMsg.c_str(), noNickMsg.size(), 0);
    return (true);
}
