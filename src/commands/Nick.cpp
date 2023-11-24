
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

    //: dan-!d@localhost NICK Mamoped
    if (parseNickname(args, clientFd))
    {
        args.erase(std::remove(args.begin(), args.end(), '\n'), args.end());
        args.erase(std::remove(args.begin(), args.end(), '\r'), args.end());
        bool hasNick = server->clients[clientFd].getNickName() != "";
        if (hasNick)
        {
            std::cout << "NICKNAME CHANGED" << std::endl;
            std::string oldNick = server->clients[clientFd].getNickName();
            std::string user = server->clients[clientFd].getUserName().empty() ? "user" : server->clients[clientFd].getUserName();

            std::stringstream ss;
            ss << ":" << oldNick << "!" << user << "@localhost NICK " << args << "\r\n";
            std::string nickMsg = ss.str();
            send(clientFd, nickMsg.c_str(), nickMsg.size(), 0);
            server->clients[clientFd].setNickName(args);
        }
        else
        {
            std::cout << "NICKNAME SET" << std::endl;
            std::stringstream ss;
            std::cout << "NICKNAME SET as" << args << std::endl;
            server->clients[clientFd].setNickName(args);
            std::string user = server->clients[clientFd].getUserName() == "" ? "user" : server->clients[clientFd].getUserName();
            std::string nickMsg = ss.str();
            server->clients[clientFd].welcomeClient(clientFd);
            send(clientFd, nickMsg.c_str(), nickMsg.size(), 0);
        }

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