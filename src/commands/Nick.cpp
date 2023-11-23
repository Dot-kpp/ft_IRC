
#include "../../inc/commands/Nick.hpp"

Nick::~Nick()
{
}

bool Nick::execute(std::string args, int clientFd)
{
    Server *server = Server::instance;

    std::string serverName = "YourServerName";
    std::string clientNick = "*";

    if (args.empty())
    {
        std::stringstream ss;
        ss << ":" << serverName << " 431 " << clientNick << " "
           << " :No nickname given\r\n";
        std::string noNickMsg = ss.str();
        send(clientFd, noNickMsg.c_str(), noNickMsg.size(), 0);
        return false;
    }

    if (parseNickname(args, clientFd))
    {
        server->clients[clientFd].setNickName(args);
        std::stringstream ss;
        ss << ":" << serverName << " 001 " << args << " "
           << " :Welcome to the Internet Relay Network " << args << "\r\n";
        std::string welcomeMsg = ss.str();
        send(clientFd, welcomeMsg.c_str(), welcomeMsg.size(), 0);
        return true;
    }
    return false;
}

bool Nick::parseNickname(std::string nickname, int clientFd)
{
    Server *server = Server::instance;

    std::string serverName = "YourServerName";
    std::string clientNick = "*";

    // Check if the nickname has more than one word
    if (nickname.find(' ') != std::string::npos)
    {
        std::stringstream ss;
        ss << ":" << serverName << " 432 " << clientNick << " "
           << " :Erroneous nickname\r\n";
        std::string erroneousNickMsg = ss.str();
        send(clientFd, erroneousNickMsg.c_str(), erroneousNickMsg.size(), 0);
        return false;
    }
    // Check if the nickname is already taken
    std::map<int, Client>::iterator it = server->clients.begin();
    while (it != server->clients.end())
    {
        if (it->second.getNickName() == nickname && it->first != clientFd)
        {
            std::cout << "NICKNAME ALREADY TAKEN" << std::endl;
            std::stringstream ss;
            ss << ":" << serverName << " 433 " << clientNick << " "
               << " :Nickname is already in use\r\n";
            std::string nickInUseMsg = ss.str();
            send(clientFd, nickInUseMsg.c_str(), nickInUseMsg.size(), 0);
            return false;
        }
        it++;
    }

    if (isdigit(nickname[0]))
    {
        std::cout << "NICKNAME STARTS WITH DIGIT" << std::endl;
        std::stringstream ss;
        ss << ":" << serverName << " 432 " << clientNick << " "
           << " :Erroneous nickname\r\n";
        std::string erroneousNickMsg = ss.str();
        send(clientFd, erroneousNickMsg.c_str(), erroneousNickMsg.size(), 0);
        return false;
    }

    // Check if the nickname contains only valid characters
    std::string trim = nickname.substr(0, nickname.size() - 2);
    for (std::string::iterator it = trim.begin(); it != trim.end(); ++it)
    {
        char c = *it;
        if (!(isalnum(c) || c == '[' || c == ']' || c == '{' || c == '}' || c == '\\' || c == '|'))
        {
            std::cout << "INVALID CHARACTER IN NICKNAME: " << static_cast<int>(c) << std::endl;
            std::stringstream ss;
            ss << ":" << serverName << " 432 " << clientNick << " " << nickname << " :Erroneous nickname\r\n";
            std::string invalidCharMsg = ss.str();
            send(clientFd, invalidCharMsg.c_str(), invalidCharMsg.size(), 0);
            return false;
        }
    }

    return true;
}