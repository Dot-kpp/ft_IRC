#include "../../inc/commands/User.hpp"

User::~User() {};

bool User::execute(std::string args, int clientFd)
{
    Server *server = Server::instance;

    if (args.empty())
    {
        std::string error = ": 461 " + std::to_string(clientFd) + " USER :Not enough parameters\r\n";
        send(clientFd, error.c_str(), error.size(), 0);
        return false;
    }
    std::cout << "User args: " << args << std::endl;
    std::string userName;
    std::string realName;
    std::string tmp;
    std::istringstream iss(args);
    iss >> userName;
    iss >> tmp;
    iss >> tmp;
    std::getline(iss, realName);
    //Check if the username and realname are valid
    if (userName.empty() || realName.empty())
    {
        std::string error = ": 461 " + std::to_string(clientFd) + " USER :Not enough parameters\r\n";
        send(clientFd, error.c_str(), error.size(), 0);
        return false;
    }
    //Check if the username and realname are valid
    if (userName.size() > 9)
    {
        std::string error = ": 461 " + std::to_string(clientFd) + " USER :Username is too long\r\n";
        send(clientFd, error.c_str(), error.size(), 0);
        return false;
    }
    //Check if the username and realname are valid
    if (realName.size() > 50)
    {
        std::string error = ": 461 " + std::to_string(clientFd) + " USER :Real name is too long\r\n";
        send(clientFd, error.c_str(), error.size(), 0);
        return false;
    }
    server->clients[clientFd].setUserName(userName);
    return true;
}

