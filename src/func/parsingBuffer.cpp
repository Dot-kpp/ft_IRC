#include "../../inc/server/Server.hpp"

bool parseNickname(std::string nickname, int clientFd)
{
    Server *server = Server::instance;

    std::map<int, Client>::iterator it = server->clients.begin();
    while (it != server->clients.end())
    {
        if (it->second.getNickName() == nickname)
        {
            std::string doubleNickError = "433 " + nickname + " :Nickname is already in use";
            send(clientFd, doubleNickError.c_str(), doubleNickError.size(), 0);
            return false;
        }
        it++;
    }
    if (isdigit(nickname[0])) {
        std::string doubleNickError = "432 " + nickname + " :Erroneus nickname";
        send(clientFd, doubleNickError.c_str(), doubleNickError.size(), 0);
        return false;
    }

    // Check if the nickname contains only valid characters
    for (std::string::iterator it = nickname.begin(); it != nickname.end(); ++it)
    {
        char c = *it;
        if (!(isalnum(c) || c == '[' || c == ']' || c == '{' || c == '}' || c == '\\' || c == '|'))
        {
            std::string doubleNickError = "432 " + nickname + " :Erroneus nickname";
            send(clientFd, doubleNickError.c_str(), doubleNickError.size(), 0);
            return false;
        }
    }

    return true;
}